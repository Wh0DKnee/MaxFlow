#pragma once
#include <vector>

struct Neighbor
{
	int index;
	int weight;
};

struct Vertex
{
	Vertex(float x, float y) : posX(x), posY(y){}

	// payload
	float posX;
	float posY;
	
	// add additional payload here

	// neighborIndex and weight
	std::vector<Neighbor> neighbors;
};


