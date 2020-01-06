#include "GraphUtils.h"
#include <chrono>

std::vector<Vertex> GraphUtils::GenerateGraph(int numNodes, int maxCapacity)
{
	static float maxWidth = 10.f;
	static float maxHeight = 10.f;

	std::vector<Vertex> graph;
	graph.reserve(numNodes);

	for (int i = 0; i < numNodes; ++i)
	{
		static std::default_random_engine e;
		e.seed(std::chrono::system_clock::now().time_since_epoch().count());
		static std::uniform_real_distribution<float> xDis(0, maxWidth);
		static std::uniform_real_distribution<float> yDis(0, maxHeight);
		float x = xDis(e);
		float y = yDis(e);

		graph.push_back(Vertex(x, y));
	}

	return graph;
}
