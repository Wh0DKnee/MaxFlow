#pragma once
#include <vector>
#include <cmath>
#include <SFML/System/Vector2.hpp>

struct RenderInfo
{
	bool isHighlighted = false;
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

	// add additional payload here

	// neighborIndex and weight
	std::vector<Neighbor> neighbors;
};


