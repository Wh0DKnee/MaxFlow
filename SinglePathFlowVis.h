#pragma once
#include "Visualizer.h"
#include "Algorithm.h"
#include <deque>

struct Edge;
class Graph;

typedef bool(*SearchAlgo)(Graph& graph, std::deque<Edge*>& outPath);
typedef void(*Algo)(Graph& graph);

class SinglePathFlowVis : public Visualizer
{
public:
	void reset() override;

	void runAlgorithm(Graph& graph) override;

	void step(Graph& graph) override;

	void setAlgo(Algorithm::AlgoType a);

private:
	enum State {search, minCap};
	State state = search;
	std::deque<Edge*> path;
	int minCapacity = 0;
	SearchAlgo searchAlgo = &Algorithm::DFS;
	Algo algo = &Algorithm::fordFulkerson;
	Algo algoStep = &Algorithm::fordFulkersonStep;
};

