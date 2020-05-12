#include "FordFulkersonVis.h"
#include "Algorithm.h"
#include "imgui.h"
#include <string>

void FordFulkersonVis::update(Graph& graph)
{
	switch (state)
	{
	case DFS:
		break;
	case minCap:
		ImGui::Begin(" ");
		ImGui::Text("minimum residual capacity on path: ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(Algorithm::getMinResidualCapacity(path)).c_str());
		ImGui::End();
		break;
	default:
		break;
	}

}

void FordFulkersonVis::reset()
{
	state = DFS;
}

void FordFulkersonVis::next(Graph& graph)
{
	switch (state)
	{
	case DFS:
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
