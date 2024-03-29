#include "FordFulkersonVis.h"
#include "Algorithm.h"
#include "imgui.h"
#include <string>

void SinglePathFlowVis::reset()
{
	Visualizer::reset();
	state = DFS;
}

void SinglePathFlowVis::runAlgorithm(Graph& graph)
{
	Algorithm::fordFulkerson(graph);
}

void SinglePathFlowVis::step(Graph& graph)
{
	switch (state)
	{
	case search:
		{
			graph.resetRenderInfo();
			if (Algorithm::DFS(graph, path))
			{
				graph.highlightPath(path);
				Edge* limitingEdge = nullptr;
				minCapacity = Algorithm::getMinResidualCapacity(path, &limitingEdge);
				assert(limitingEdge != nullptr);
				limitingEdge->setColor(sf::Color(140, 0, 186));
				state = minCap;
			}
			break;
		}
	case minCap:
		Algorithm::fordFulkersonStep(graph);
		state = DFS;
		break;
	default:
		break;
	}
}