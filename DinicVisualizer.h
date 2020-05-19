#pragma once
#include "Visualizer.h"

class DinicVisualizer : public Visualizer
{
public:
	void reset() override;

	virtual void step() override;

	virtual void runAlgorithm() override;

private:
	enum State { LEVEL_GRAPH, BLOCKING_FLOW };
	State state = LEVEL_GRAPH;
};

