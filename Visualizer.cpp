#include "Visualizer.h"

void Visualizer::update(Graph& graph, float deltaTime)
{
	if (doAutoStep)
	{
		stepTimer += deltaTime;
		if (stepTimer > autoStepDelay)
		{
			step(graph);
			stepTimer -= autoStepDelay;
		}
	}
}

void Visualizer::autoStep(Graph& graph)
{
	doAutoStep = true;
	stepTimer = autoStepDelay; // When we first do autostep, we want the first step to happen immediately.
}

void Visualizer::reset()
{
	doAutoStep = false;
	stepTimer = autoStepDelay;
}
