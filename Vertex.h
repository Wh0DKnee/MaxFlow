#pragma once
#include <vector>
#include <cmath>

struct Point
{
	Point(float x_, float y_) : x(x_), y(y_){}
	float x;
	float y;

	Point operator-(const Point& rhs) const
	{
		return Point(x - rhs.x, y - rhs.y);
	}

	Point operator+(const Point& rhs) const
	{
		return Point(x + rhs.x, y + rhs.y);
	}

	Point operator/(float div) const
	{
		return Point(x / div, y / div);
	}

	Point operator*(float div) const
	{
		return Point(x * div, y * div);
	}

	// returns normalized version, not in place
	Point normalize() const
	{
		return Point(*this / std::sqrt(x * x + y * y));
	}
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


