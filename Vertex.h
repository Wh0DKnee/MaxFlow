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

struct Neighbor
{
	Neighbor(size_t ind, int c) : index(ind), capacity(c){}

	size_t index;
	int capacity;
	int flow = 0;

	int getRemainingCapacity() const { return capacity - flow; }
};

struct Vertex
{
	Vertex(float x, float y) : pos(x, y){}
	Vertex(sf::Vector2f p) : pos(p){}

	// payload
	sf::Vector2f pos;
	
	RenderInfo renderInfo;

	// add additional payload here

	// neighborIndex and weight
	std::vector<Neighbor> neighbors;
};


