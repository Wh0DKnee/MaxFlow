#include "Graph.h"
#include "GraphUtils.h"
#include <chrono>
#include <cassert>

Graph::Graph(int numNodes, int maxCapacity, int windowWidth, int windowHeight)
{
	static int margin = 20;
	vertices.reserve(numNodes);

	static std::default_random_engine randEngine;
	static std::uniform_real_distribution<float> xDis(static_cast<float>(0 + margin), static_cast<float>(windowWidth - margin));
	static std::uniform_real_distribution<float> yDis(static_cast<float>(0 + margin), static_cast<float>(windowHeight - margin));
	randEngine.seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));

	for (int i = 0; i < numNodes; ++i)
	{
		float x = xDis(randEngine);
		float y = yDis(randEngine);

		vertices.push_back(Vertex(x, y));
	}

	std::vector<std::pair<size_t, size_t>> vertexPairs;
	for (size_t i = 0; i < size() - 1; ++i)
	{
		for (size_t j = i + 1; j < size(); ++j)
		{
			vertexPairs.push_back(std::make_pair(i, j));
		}
	}

	// sort vertex pairs by distance
	std::sort(vertexPairs.begin(), vertexPairs.end(), [&](std::pair<size_t, size_t> lhs, std::pair<size_t, size_t> rhs)
		{
			float lhsDeltaY = vertices[lhs.second].pos.y - vertices[lhs.first].pos.y;
			float lhsDeltaX = vertices[lhs.second].pos.x - vertices[lhs.first].pos.x;
			float lhsDistSquared = (lhsDeltaY * lhsDeltaY) + (lhsDeltaX * lhsDeltaX);

			float rhsDeltaY = vertices[rhs.second].pos.y - vertices[rhs.first].pos.y;
			float rhsDeltaX = vertices[rhs.second].pos.x - vertices[rhs.first].pos.x;
			float rhsDistSquared = (rhsDeltaY * rhsDeltaY) + (rhsDeltaX * rhsDeltaX);

			return lhsDistSquared < rhsDistSquared;
		});

	long long edgeCount = 0;

	for (auto& pair : vertexPairs)
	{
		bool intersectsAny = false;
		for (auto& vert : vertices)
		{
			for (auto& neighbor : vert.neighbors)
			{
				if (GraphUtils::intersectWithWidth(vertices[pair.first].pos, vertices[pair.second].pos, vert.pos, vertices[neighbor.index].pos, 0.f))
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

		vertices[pair.first].neighbors.emplace_back(pair.second, 10);
		//graph[pair.second].neighbors.emplace_back(pair.first, 10);
		++edgeCount;
	}

	setCapacitiesRandomly(edgeCount, maxCapacity);
}

void Graph::setCapacitiesRandomly(long long edgeCount, int maxCapacity)
{
	std::default_random_engine randEngine;
	randEngine.seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
	static std::uniform_real_distribution<float> capacityDist(0.f, 1.f);

	std::vector<float> randFloats;
	std::vector<int> randCapacities;
	while (true)
	{
		randFloats.clear();
		randFloats.reserve(edgeCount);
		randCapacities.clear();
		randCapacities.reserve(edgeCount);
		float sum = 0;
		for (int i = 0; i < edgeCount; ++i)
		{
			float rnd = capacityDist(randEngine);
			sum += rnd;
			randFloats.push_back(rnd);
		}

		for (auto& f : randFloats)
		{
			f /= sum;
			f *= maxCapacity;
			randCapacities.push_back(static_cast<int>(f + 0.5f));
		}

		int capacitySum = 0;
		for (const auto& i : randCapacities)
		{
			capacitySum += i;
		}

		if (capacitySum == maxCapacity) // Do rounded results add up to maxCapacity or did we get unlucky with rounding?
		{
			break;
		}
	}

	size_t index = 0;
	for (auto& vert : vertices)
	{
		for (auto& neighbor : vert.neighbors)
		{
			assert(index < randCapacities.size());
			neighbor.setCapacity(randCapacities[index]);
			++index;
		}
	}

	// We don't want to add edges while we iterate the actual graph,
	// because that might invalidate the iterator and bring us to UB land.
	// Also, it would not be the behavior we want (we dont want backward
	// edges for backward edges).
	auto nonResidualGraph = *this;

	index = 0;
	for (auto& vert : nonResidualGraph)
	{
		for (auto& neighbor : vert.neighbors)
		{
			vertices[neighbor.index].neighbors.emplace_back(index, neighbor.getCapacity(), neighbor.getCapacity());
		}
		++index;
	}
}
