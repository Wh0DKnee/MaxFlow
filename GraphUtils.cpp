#include "GraphUtils.h"
#include <chrono>
#include <algorithm>

float GraphUtils::CCW(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c)
{
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool GraphUtils::intersect(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f d)
{
	return (CCW(a, b, c) * CCW(a, b, d) < 0 && CCW(c, d, b) * CCW(c, d, a) < 0);
}

std::vector<Vertex> GraphUtils::GenerateGraph(int numNodes, int maxCapacity, int windowWidth, int windowHeight)
{
	static int margin = 20;
	std::vector<Vertex> graph;
	graph.reserve(numNodes);

	for (int i = 0; i < numNodes; ++i)
	{
		static std::default_random_engine e;
		e.seed(std::chrono::system_clock::now().time_since_epoch().count());
		static std::uniform_real_distribution<float> xDis(0 + margin, windowWidth - margin);
		static std::uniform_real_distribution<float> yDis(0 + margin, windowHeight - margin);
		float x = xDis(e);
		float y = yDis(e);

		graph.push_back(Vertex(x, y));
	}

	std::vector<std::pair<size_t, size_t>> vertexPairs;
	for (size_t i = 0; i < graph.size() - 1; ++i)
	{
		for (size_t j = i + 1; j < graph.size(); ++j)
		{
			vertexPairs.push_back(std::make_pair(i, j));
		}
	}

	// sort vertex pairs by distance
	std::sort(vertexPairs.begin(), vertexPairs.end(), [&](std::pair<size_t, size_t> lhs, std::pair<size_t, size_t> rhs)
		{
			float lhsDeltaY = graph[lhs.second].pos.y - graph[lhs.first].pos.y;
			float lhsDeltaX = graph[lhs.second].pos.x - graph[lhs.first].pos.x;
			float lhsDistSquared = (lhsDeltaY * lhsDeltaY) + (lhsDeltaX * lhsDeltaX);

			float rhsDeltaY = graph[rhs.second].pos.y - graph[rhs.first].pos.y;
			float rhsDeltaX = graph[rhs.second].pos.x - graph[rhs.first].pos.x;
			float rhsDistSquared = (rhsDeltaY * rhsDeltaY) + (rhsDeltaX * rhsDeltaX);

			return lhsDistSquared < rhsDistSquared;
		});

	for (auto& pair : vertexPairs)
	{
		bool intersectsAny = false;
		for (auto& vert : graph)
		{
			for (auto& neighbor : vert.neighbors)
			{
				if (intersect(graph[pair.first].pos, graph[pair.second].pos, vert.pos, graph[neighbor.index].pos))
				{
					intersectsAny = true;
					break;
				}
			}
			if (intersectsAny)
				break;
		}
		if (intersectsAny)
		{
			continue; // cant insert without intersection, next.
		}

		graph[pair.first].neighbors.emplace_back(pair.second, 0);
		graph[pair.second].neighbors.emplace_back(pair.first, 0);
	}


	return graph;
}
