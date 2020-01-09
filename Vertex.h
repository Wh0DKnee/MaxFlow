#pragma once
#include <vector>

struct Point
{
	Point(float x_, float y_) : x(x_), y(y_){}
	float x;
	float y;
};

struct Neighbor
{
	Neighbor(int ind, int w) : index(ind), weight(w){}
	int index;
	int weight;
};

struct Vertex
{
	Vertex(float x, float y) : pos(x, y){}
	Vertex(Point p) : pos(p){}

	// payload
	Point pos;
	
	// add additional payload here

	// neighborIndex and weight
	std::vector<Neighbor> neighbors;
};


