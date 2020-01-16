#include "GraphUtils.h"
#include "VectorUtils.h"
#include <chrono>
#include <algorithm>
#include <array>

float GraphUtils::CCW(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c)
{
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool GraphUtils::intersect(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f d)
{
	return (CCW(a, b, c) * CCW(a, b, d) < 0 && CCW(c, d, b) * CCW(c, d, a) < 0);
}

// Checks if two lines with a width (read: rectangles) intersect using separating axis theorem.
bool GraphUtils::intersectWithWidth(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f d, float width)
{
	if (width <= 0.f)
	{
		return intersect(a, b, c, d);
	}

	// TEST BEGIN

	static const float offset = 40.f;
	auto bToA = b - a;
	auto cToD = d - c;
	a = a + VectorUtils::normalize(bToA) * offset;
	b = b - VectorUtils::normalize(bToA) * offset;
	c = c + VectorUtils::normalize(cToD) * offset;
	d = d - VectorUtils::normalize(cToD) * offset;
	

	// TEST END


	float halfWidth = width / 2.f;
	std::array<std::array<sf::Vector2f, 4>, 2> rects;
	sf::Vector2f line1Dir = b - a;
	sf::Vector2f line1Perpendicular = sf::Vector2f(-line1Dir.y, line1Dir.x);
	sf::Vector2f line1PerpendicularNorm = VectorUtils::normalize(line1Perpendicular);
	rects[0] = { a - line1PerpendicularNorm * halfWidth, a + line1PerpendicularNorm * halfWidth,
							b + line1PerpendicularNorm * halfWidth, b - line1PerpendicularNorm * halfWidth };

	sf::Vector2f line2Dir = d - c;
	sf::Vector2f line2Perpendicular = sf::Vector2f(-line2Dir.y, line2Dir.x);
	sf::Vector2f line2PerpendicularNorm = VectorUtils::normalize(line2Perpendicular);
	rects[1] = { c - line2PerpendicularNorm * halfWidth, c + line2PerpendicularNorm * halfWidth,
							d + line2PerpendicularNorm * halfWidth, d - line2PerpendicularNorm * halfWidth };

	for (const auto& rect : rects)
	{
		for (int i = 0; i < 4; ++i)
		{
			int i2 = (i + 1) % 4;
			sf::Vector2f face = rect[i2] - rect[i];
			sf::Vector2f facePerpendicularNorm = VectorUtils::normalize(sf::Vector2f(-face.y, face.x));
		
			std::vector<float> rect1Projections;
			for (auto& p : rects[0])
			{
				rect1Projections.push_back(VectorUtils::dot(facePerpendicularNorm, p));
			}
			std::vector<float> rect2Projections;
			for (auto& p : rects[1])
			{
				rect2Projections.push_back(VectorUtils::dot(facePerpendicularNorm, p));
			}

			float min1 = *std::min_element(rect1Projections.begin(), rect1Projections.end());
			float max1 = *std::max_element(rect1Projections.begin(), rect1Projections.end());

			float min2 = *std::min_element(rect2Projections.begin(), rect2Projections.end());
			float max2 = *std::max_element(rect2Projections.begin(), rect2Projections.end());

			if (!(max1 < min2 || max2 < min1))
			{
				return true; // intersection
			}
		}
	}

	return false;
}



std::vector<Vertex> GraphUtils::generateGraph(int numNodes, int maxCapacity, int windowWidth, int windowHeight)
{
	static int margin = 20;
	std::vector<Vertex> graph;
	graph.reserve(numNodes);

	static std::default_random_engine randEngine;
	static std::uniform_real_distribution<float> xDis(static_cast<float>(0 + margin), static_cast<float>(windowWidth - margin));
	static std::uniform_real_distribution<float> yDis(static_cast<float>(0 + margin), static_cast<float>(windowHeight - margin));
	randEngine.seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));

	for (int i = 0; i < numNodes; ++i)
	{
		float x = xDis(randEngine);
		float y = yDis(randEngine);

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

	long long edgeCount = 0;

	for (auto& pair : vertexPairs)
	{
		bool intersectsAny = false;
		for (auto& vert : graph)
		{
			for (auto& neighbor : vert.neighbors)
			{
				if (intersectWithWidth(graph[pair.first].pos, graph[pair.second].pos, vert.pos, graph[neighbor.index].pos, 0.f))
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

		graph[pair.first].neighbors.emplace_back(pair.second, 10);
		graph[pair.second].neighbors.emplace_back(pair.first, 10);
		++edgeCount;
	}

	setCapacitiesRandomly(graph, edgeCount, maxCapacity);


	return graph;
}

void GraphUtils::setCapacitiesRandomly(std::vector<Vertex>& graph, long long edgeCount, int maxCapacity)
{
	std::default_random_engine randEngine;
	randEngine.seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
	static std::uniform_real_distribution<float> capacityDist(0.f, 1.f);

	std::vector<float> randZeroToOne;
	randZeroToOne.reserve(edgeCount);
	float sum = 0;
	for (int i = 0; i < edgeCount; ++i)
	{
		float rnd = capacityDist(randEngine);
		sum += rnd;
		randZeroToOne.push_back(rnd);
	}

	for (auto& f : randZeroToOne)
	{
		f /= sum;
	}

}
