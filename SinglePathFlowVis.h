#pragma once
#include "Visualizer.h"
#include "Algorithm.h"
#include <deque>

struct Edge;

typedef GraphSearchResult (*SearchAlgo)(Graph& graph, std::deque<Edge*>& outPath);
typedef void(*Algo)(Graph& graph);

class SinglePathFlowVis : public Visualizer
{
public:
	void reset() override;

	void runAlgorithm() override;

	void step() override;

	void setAlgo(Algorithm::AlgoType a);

private:
	enum State {SEARCH, MIN_CAP};
	State state = SEARCH;
	std::deque<Edge*> path;
	int minCapacity = 0;
	SearchAlgo searchAlgo = &Algorithm::DFS;
	Algo algo = &Algorithm::fordFulkerson;
	Algo algoStep = &Algorithm::fordFulkersonStep;
};

