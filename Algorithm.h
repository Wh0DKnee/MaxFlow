#pragma once
#include <vector>
#include <deque>
#include "Vertex.h"
#include "Graph.h"

namespace Algorithm
{
	// TODO: figure out how to make graph a const ref,
	// the problem is that we need to output a vector
	// of non-const Edge ptrs so that FF can modify them.
	bool DFS(Graph& graph, std::deque<Edge*>& outPath);

	void traceBack(const std::vector<Edge*>& visitedFrom, size_t target, std::deque<Edge*>& outPath);

	void fordFulkerson(Graph& graph);
};

