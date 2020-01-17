#pragma once
#include <vector>
#include <deque>
#include "Vertex.h"
#include "Graph.h"

namespace Algorithm
{
	// TODO: store start and target in Graph and dont pass it.
	std::deque<size_t> DFS(const Graph& graph, size_t start, size_t target);

	std::deque<size_t> traceBack(const std::vector<size_t>& visitedFrom, size_t target);
};

