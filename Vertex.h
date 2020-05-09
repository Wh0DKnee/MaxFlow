#pragma once
#include <vector>
#include <list>
#include <cmath>
#include <SFML/System/Vector2.hpp>
#include <limits>

struct RenderInfo
{
	bool isHighlighted = false;
	
	void reset()
	{
		isHighlighted = false;
	}
};

struct Edge
{
	Edge(size_t startInd, size_t targetInd, int c, int f = 0) : startNode(startInd), targetNode(targetInd), capacity(c), flow(f) {}
	
	// TODO: Rethink why we store these as indexes and not as references/pointers. Maybe there was a reason for it but I forgot.
	// Probably because pointers would point into a vector and if that changes the pointers would be invalidated. Could use a std::list
	// instead to avoid reallocation, but would lose performance.
	size_t startNode;
	size_t targetNode;

	RenderInfo renderInfo;

	int getRemainingCapacity() const { return capacity - flow; }
	void setCapacity(int c) { capacity = c; }
	int getCapacity() const { return capacity; }

	int getFlow() const { return flow; }

	// Adds flow to edge
	void addResidualFlow(int amount);

	void setBackwardEdge(Edge* b) { backward = b; }

	void setCombinedEdge(Edge* c) { combined = c; }

private:
	int flow;
	int capacity;

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
	
	RenderInfo renderInfo;

	// add additional payload here

	// outgoing edges
	std::vector<Edge> edges;

	// combined edges for rendering only (don't wanna draw four edges between each vertex pair)
	std::list<Edge> renderedEdges;
};


