#include "DinicVisualizer.h"
#include "Algorithm.h"
#include <cassert>

void DinicVisualizer::reset()
{
	Visualizer::reset();
	state = LEVEL_GRAPH;
}

void DinicVisualizer::step()
{
	assert(graph != nullptr);

	switch (state)
	{
	case DinicVisualizer::LEVEL_GRAPH:
		{
			graph->resetRenderInfo();
			graph->resetDinicLevels();
			std::deque<Edge*> path;
		
			if (Algorithm::BFS(*graph, path))
			{
				size_t index = 0;
				for (auto& v : *graph)
				{
					if (graph->getLevel(index) < 0)	// if vertex not part of level graph...
					{
													// ... fade it out
						v.renderInfo.setColor(sf::Color(179, 179, 179));
					}

					for (auto& e : v.edges)
					{
						if (!e.getCombinedEdge()->isInLevelGraph(*graph))
						{
							e.setColor(sf::Color(179, 179, 179));
						}
					}

					++index;
				}
				state = BLOCKING_FLOW;
			}
		}
		break;
	case DinicVisualizer::BLOCKING_FLOW:
		Algorithm::dinicStep(*graph);
		state = LEVEL_GRAPH;
		break;
	default:
		break;
	}
}

void DinicVisualizer::runAlgorithm()
{
	if (graph != nullptr)
	{
		Algorithm::dinic(*graph);
	}
}
