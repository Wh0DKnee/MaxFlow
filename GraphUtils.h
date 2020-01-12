#pragma once

#include <vector>
#include <random>
#include "Vertex.h"

namespace GraphUtils
{
	float CCW(Point a, Point b, Point c);

	bool intersect(Point a, Point b, Point c, Point d);

	std::vector<Vertex> GenerateGraph(int numNodes, int maxCapacity, int windowWidth, int windowHeight);
};

