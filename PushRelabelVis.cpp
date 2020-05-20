#include "PushRelabelVis.h"
#include "Algorithm.h"
#include <cassert>

const float PushRelabelVis::nodeRenderRadius = 10.f;

void PushRelabelVis::reset()
{
	Visualizer::reset();
	isFirstStep = true;
}

void PushRelabelVis::step()
{
	assert(graph != nullptr);

	if (isFirstStep)
	{
		isFirstStep = false;
		Algorithm::pushRelabelInit(*graph);

		for (auto& v : *graph)
		{
			v.renderInfo.radius = nodeRenderRadius;
			v.renderInfo.labelType = VertexRenderInfo::LabelType::HEIGHT;
		}
	}
	else
	{
		Algorithm::pushRelabelStep(*graph);
	}
}

void PushRelabelVis::runAlgorithm()
{
	if (graph != nullptr)
	{
		Algorithm::pushRelabel(*graph);
	}
}
