#pragma once
#include <vector>
#include <deque>
#include "Vertex.h"

namespace Algorithm
{
	std::deque<size_t> DFS(const std::vector<Vertex>& graph, size_t start, size_t target);

	std::deque<size_t> traceBack(const std::vector<size_t>& visitedFrom, size_t target);
};

