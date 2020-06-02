#include "PushRelabelVis.h"
#include "Algorithm.h"
#include <cassert>

const float PushRelabelVis::nodeRenderRadius = 11.5f;
const float PushRelabelVis::nodeSquareRenderLength = 23.f;

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
			v.renderInfo.squareLength = nodeSquareRenderLength;
			v.renderInfo.labelType = VertexRenderInfo::LabelType::HEIGHT;
		}
	}
	else
	{
		// This is hideous code.
		if (graph->verticesWithExcess.empty())
		{
			if (colorEqualIgnoreAlpha((*graph)[previousActive].renderInfo.getColor(), activeColor))
			{
				(*graph)[previousActive].renderInfo.setColor((*graph)[previousActive].renderInfo.getRegularColor());
			}
		}

		active = graph->verticesWithExcess.front();
		if (active != previousActive)
		{
			if (colorEqualIgnoreAlpha((*graph)[previousActive].renderInfo.getColor(), activeColor))
			{
				(*graph)[previousActive].renderInfo.setColor((*graph)[previousActive].renderInfo.getRegularColor());
			}
			(*graph)[active].renderInfo.setColor(activeColor);
		}
		else
		{
			Algorithm::pushRelabelStep(*graph);
		}
		previousActive = active;
	}

	saturatedCutVis();
}

void PushRelabelVis::runAlgorithm()
{
	if (graph != nullptr)
	{
		Algorithm::pushRelabel(*graph);
		saturatedCutVis();
	}
}

void PushRelabelVis::saturatedCutVis()
{
	assert(graph != nullptr);

	std::deque<Edge*> path;
	auto searchResult = Algorithm::BFS(*graph, path);

	size_t index = 0;
	for (auto& v : *graph)
	{
		auto color = v.renderInfo.getColor();
		if (searchResult.visited[index])
		{
			//color.a = 255;
			v.renderInfo.shape = VertexRenderInfo::Shape::CIRCLE;
		}
		else
		{
			v.renderInfo.shape = VertexRenderInfo::Shape::RECT;
			//color.a = 120;
		}
		v.renderInfo.setColor(color);
		++index;
	}
}

bool colorEqualIgnoreAlpha(const sf::Color& lhs, const sf::Color& rhs)
{
	return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}
