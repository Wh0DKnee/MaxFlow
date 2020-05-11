#pragma once
#include "Visualizer.h"
#include <deque>

struct Edge;
class Graph;

class FordFulkersonVis : public Visualizer
{
public:
	void update(Graph& graph) override;

	void reset() override;

	void next(Graph& graph);

private:
	enum State {DFS, minCap, exhaustPath};
	State state = DFS;
	std::deque<Edge*> path;
	int minCapacity = 0;
};

