#pragma once
#include <vector>
#include <cmath>
#include <SFML/System/Vector2.hpp>
#include <limits>

struct RenderInfo
{
	bool isHighlighted = false;
	size_t successor = std::numeric_limits<size_t>::max();

	bool hasSuccessor() const { return successor != std::numeric_limits<size_t>::max(); }
	
	void reset()
	{
		isHighlighted = false;
		successor = std::numeric_limits<size_t>::max();
	}
};

struct Edge
{
	Edge(size_t ind, int c, int f = 0) : targetNode(ind), capacity(c), flow(f){}

	size_t targetNode;
	int flow;

	int getRemainingCapacity() const { return capacity - flow; }
	void setCapacity(int c) { capacity = c; }
	int getCapacity() { return capacity; }

	void addResidualFlow(int amount);

private:
	int capacity;
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


