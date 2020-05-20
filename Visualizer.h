#pragma once

class Graph;

class Visualizer
{
public:
	void setGraph(Graph& g) { graph = &g; }

	virtual void update(float deltaTime);

	virtual void step() = 0;

	virtual void autoStep();

	virtual void reset();

	virtual void runAlgorithm() = 0;

	void setAutoStepDelay(float d) { autoStepDelay = d; }

protected:
	Graph* graph = nullptr;

private:
	bool doAutoStep = false;
	float autoStepDelay = 0.f;
	float stepTimer = 0.f;
};

