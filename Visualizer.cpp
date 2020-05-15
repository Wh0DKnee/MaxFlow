#include "Visualizer.h"
#include "Graph.h"

void Visualizer::update(float deltaTime)
{
	if (doAutoStep)
	{
		stepTimer += deltaTime;
		if (stepTimer > autoStepDelay)
		{
			step();
			stepTimer -= autoStepDelay;
		}
	}
}

void Visualizer::autoStep()
{
	doAutoStep = true;
	stepTimer = autoStepDelay; // When we first do autostep, we want the first step to happen immediately.
}

void Visualizer::reset()
{
	doAutoStep = false;
	stepTimer = autoStepDelay;
	if (graph != nullptr)
	{
		graph->resetRenderInfo();
	}
}
