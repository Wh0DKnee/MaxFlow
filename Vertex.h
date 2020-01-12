#pragma once
#include <vector>
#include <cmath>
#include <SFML/System/Vector2.hpp>

struct Neighbor
{
	Neighbor(int ind, int w) : index(ind), weight(w){}
	int index;
	int weight;
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


