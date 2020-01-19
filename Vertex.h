#pragma once
#include <vector>
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

	size_t startNode;
	size_t targetNode;

	RenderInfo renderInfo;

	int getRemainingCapacity() const { return capacity - flow; }
	void setCapacity(int c) { capacity = c; }
	int getCapacity() { return capacity; }

	int getFlow() { return flow; }

	void addResidualFlow(int amount);

	void setBackwardEdge(Edge* b) { backward = b; }

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
	  to access the targetNode.*/
	Edge* backward = nullptr;

	void addFlow(int amount) { flow += amount; }
};

struct Vertex
{
	Vertex(float x, float y) : pos(x, y){}
	Vertex(sf::Vector2f p) : pos(p){}

	// payload
	sf::Vector2f pos;
	
	RenderInfo renderInfo;

	// add additional payload here

	std::vector<Edge> edges;
};


