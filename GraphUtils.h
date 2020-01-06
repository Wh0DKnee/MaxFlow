#pragma once

#include <vector>
#include <random>
#include "Vertex.h"

namespace GraphUtils
{
	std::vector<Vertex> GenerateGraph(int numNodes, int maxCapacity);
};

