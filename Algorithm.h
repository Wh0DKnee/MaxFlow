#pragma once
#include <vector>
#include <deque>
#include "Vertex.h"
#include "Graph.h"

typedef bool (*SearchAlgo) (Graph& graph, std::deque<Edge*>& outPath);

namespace Algorithm
{
	enum AlgoType { FORD_FULKERSON, EDMONDS_KARP };

	// TODO: figure out how to make graph a const ref,
	// the problem is that we need to output a vector
	// of non-const Edge ptrs so that FF can modify them.
	bool DFS(Graph& graph, std::deque<Edge*>& outPath);

	bool BFS(Graph& graph, std::deque<Edge*>& outPath);

	bool dinicDFS(Graph& graph, std::deque<Edge*>& outPath);

	void traceBack(const std::vector<Edge*>& visitedFrom, size_t target, std::deque<Edge*>& outPath);

	void edmondsKarp(Graph& graph);

	void edmondsKarpStep(Graph& graph);

	void fordFulkerson(Graph& graph);

	void fordFulkersonStep(Graph& graph);

	void dinic(Graph& graph);

	void dinicStep(Graph& graph);

	void dinicBlockingFlow(Graph& graph);

	void exhaustPath(std::deque<Edge*>& path);

	int getMinResidualCapacity(const std::deque<Edge*>& path, Edge** outLimitingEdge = nullptr);

	void pushRelabel(Graph& graph);

	//TODO: refactor push and pushRelabelInit, as pushRelabelInit is really just a bunch of pushes.
	void push(Graph& graph, Edge& edge, int excess);

	void pushRelabelInit(Graph& graph);
};

