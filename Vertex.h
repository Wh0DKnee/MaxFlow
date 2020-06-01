#pragma once
#include <vector>
#include <list>
#include <cmath>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include "RenderInfo.h"
#include "UIConfig.h"

/*
	The data structure is a bit complicated, so here is how it works:

	Each connected node pair has 6 edges connecting them, three in each direction.
	The original graph has one edge in each direction, hence the residual graph will have
	two in each - that makes four. Since we can't render four edges between each node pair
	(unreadable), we combine the edges in the residual graph for visualization purposes.
	We still need to keep the actual residual graph for the algorithms though.

	So, each edge in the residual graph has a pointer to its combined edge that it's a part of,
	so that it can update the data in the combined edge whenever it changes.

	TOOD: Update comment.

	The whole point of the residual graph is that we don't need to differ between forward and
	backward edges, which simplifies the algorithms. However, we still tag each edge whether it
	was in the residual graph, because that's the only way to visualize flow reasonably (or I 
	missed a better data structure for the edges). Backward edges are represented with the same
	data structure as forward edges, with a capacity, a flow, and a resulting remaining capacity
	of (capacity - flow). So, if we initialize forward edges with zero flow, backward edges need
	to be initialize with full flow, so that the initial remaining capacity results in zero.
	Therefore, if we were to just visualize the value of the flow member, backward edges would 
	have a maximum value at the start, which is misleading. In essence, backward edges simply
	represent how much flow we can take off of the forward edge. Flow on a backward edge is an
	artificial concept.

	Long story short: We tag edges with an isOriginal flag, so that we only visualize flow on 
	forward edges.
*/

class Graph;

struct Edge
{
	Edge(size_t startInd, size_t targetInd, int c, bool isOrig = false) 
		: startNode(startInd), targetNode(targetInd), residualCap(c), isOriginal(isOrig) {}
	
	// TODO: Rethink why we store these as indexes and not as references/pointers. Maybe there was a reason for it but I forgot.
	// Probably because pointers would point into a vector and if that changes the pointers would be invalidated. Could use a std::list
	// instead to avoid reallocation, but would lose performance.
	size_t startNode;
	size_t targetNode;

	bool isOriginal = false;

	// Visualized flow, only meaningful for combined edges.
	int renderedFlow = 0;

	RenderInfo renderInfo;

	int getResidualCapacity() const { return residualCap; }
	void addCapacity(int c);

	int getFlow() const;

	// Adds flow to edge
	void addResidualFlow(int amount);

	void setBackwardEdge(Edge* b) { backward = b; }

	Edge* const getBackwardEdge() const { return backward; }

	void setCombinedEdge(Edge* c) { combined = c; }

	Edge const* const getCombinedEdge() const { return combined; }

	void highlight();

	void resetHighlight();

	void setColor(sf::Color c);

	// I don't like passing a graph ref in here, but I think the only
	// alternative is to store a Graph ref or ptr in every Vertex/Edge,
	// which is a bit of a memory waste really.
	bool isInLevelGraph(const Graph& graph) const;

private:
	int residualCap;

	/*Pointer to the edge in reverse direction.
	  This is safe, because once a graph is created, it is static.
	  This means that the edge vector in each Vertex wont change,
	  so we can safely store a pointer into it. During the lifetime 
	  of a graph, no edges are added, and with its destruction, this
	  object is also destructed.
	  We could instead also store the index into the targetNode's edges
	  vector, but for that we'd have to store a reference to the graph
	  to access the targetNode.
	  We could also just use a std::list for edges, which doesn't invalidate
	  iterators/pointers when inserting or deleting elements.*/
	Edge* backward = nullptr;

	// Pointer to edge that gets rendered and combines the edge pair
	// with equal start and target vertices.
	Edge* combined = nullptr;

	void addFlow(int amount);
};

struct Vertex
{
	Vertex(float x, float y) : pos(x, y){}
	Vertex(sf::Vector2f p) : pos(p){}

	// payload
	sf::Vector2f pos;
	
	VertexRenderInfo renderInfo;

	// add additional payload here

	// outgoing edges
	std::vector<Edge> edges;

	// combined edges for rendering only (don't wanna draw four edges between each vertex pair)
	std::list<Edge> renderedEdges;

	void setExcess(int e) { excess = e; }
	void addExcess(int e) { excess += e; }
	int getExcess() const { return excess; }

	void setHeight(int h) { height = h; }
	void incrementHeight() { ++height; }
	int getHeight() const { return height; }

	int getOutGoingFlow() const;
	int getIncomingFlow() const;

private:
	int excess = 0;

	int height = 0;
};


