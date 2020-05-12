#pragma once
#include "Visualizer.h"
#include <deque>

struct Edge;
class Graph;

class FordFulkersonVis : public Visualizer
{
public:
	void reset() override;

	void runAlgorithm(Graph& graph) override;

	void step(Graph& graph) override;

private:
	enum State {DFS, minCap, exhaustPath};
	State state = DFS;
	std::deque<Edge*> path;
	int minCapacity = 0;
};

