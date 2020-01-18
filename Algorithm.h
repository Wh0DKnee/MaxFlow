#pragma once
#include <vector>
#include <deque>
#include "Vertex.h"
#include "Graph.h"

namespace Algorithm
{
	std::deque<size_t> DFS(const Graph& graph);

	std::deque<size_t> traceBack(const std::vector<size_t>& visitedFrom, size_t target);

	void fordFulkerson(Graph& graph);
};

