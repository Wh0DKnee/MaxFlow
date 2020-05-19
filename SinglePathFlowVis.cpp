#include "SinglePathFlowVis.h"
#include <cassert>
#include <string>

void SinglePathFlowVis::reset()
{
	Visualizer::reset();
	state = SEARCH;
}

void SinglePathFlowVis::runAlgorithm()
{
	if (graph != nullptr)
	{
		algo(*graph);
	}
}

void SinglePathFlowVis::step()
{
	assert(graph != nullptr);

	switch (state)
	{
	case SEARCH:
		{
			graph->resetRenderInfo();
			if (searchAlgo(*graph, path))
			{
				graph->highlightPath(path);
				Edge* limitingEdge = nullptr;
				minCapacity = Algorithm::getMinResidualCapacity(path, &limitingEdge);
				assert(limitingEdge != nullptr);
				limitingEdge->setColor(sf::Color(140, 0, 186));
				state = MIN_CAP;
			}
			break;
		}
	case MIN_CAP:
		algoStep(*graph);
		state = SEARCH;
		break;
	default:
		break;
	}
}

void SinglePathFlowVis::setAlgo(Algorithm::AlgoType a)
{
	switch (a)
	{
	case Algorithm::AlgoType::FORD_FULKERSON:
		searchAlgo = &Algorithm::DFS;
		algo = &Algorithm::fordFulkerson;
		algoStep = &Algorithm::fordFulkersonStep;
		break;
	case Algorithm::AlgoType::EDMONDS_KARP:
		searchAlgo = &Algorithm::BFS;
		algo = &Algorithm::edmondsKarp;
		algoStep = &Algorithm::edmondsKarpStep;
		break;
	default:
		break;
	}
}
