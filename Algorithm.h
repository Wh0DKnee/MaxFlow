#pragma once
#include <vector>
#include "Vertex.h"

namespace Algorithm
{
	std::vector<size_t> DFS(const std::vector<Vertex>& graph, size_t start, size_t target);
};

