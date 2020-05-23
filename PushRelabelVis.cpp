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
		std::deque<Edge*> path;
		auto searchResult = Algorithm::BFS(*graph, path);

		size_t index = 0;
		for (auto& v : *graph)
		{
			auto color = v.renderInfo.getColor();
			if (searchResult.visited[index])
			{
				color.a = 255;
			}
			else
			{
				color.a = 170;
			}
			v.renderInfo.setColor(color);
			++index;
		}

	}
}

void PushRelabelVis::runAlgorithm()
{
	if (graph != nullptr)
	{
		Algorithm::pushRelabel(*graph);
	}
}
