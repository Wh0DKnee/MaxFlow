#pragma once

class Graph;

class Visualizer
{
public:
	// Should we pass graph into functions, or have a Graph* as member? We'd have to remember to re-set
	// the graph when we re-generate it. I think the answer is yes.
	// TODO: do this.

	virtual void update(Graph& graph, float deltaTime);

	virtual void step(Graph& graph) = 0;

	virtual void autoStep(Graph& graph);

	virtual void reset();

	virtual void runAlgorithm(Graph& graph) = 0;

	void setAutoStepDelay(float d) { autoStepDelay = d; }

private:

	bool doAutoStep = false;
	float autoStepDelay = 0.f;
	float stepTimer = 0.f;
};

