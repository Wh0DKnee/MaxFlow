#include "Graph.h"
#include "GraphUtils.h"
#include <chrono>
#include <cassert>
#include "Algorithm.h"

Graph::Graph(int numNodes, int maxCapacity, int windowWidth, int windowHeight, float minDist) : minDist(minDist)
{
	static int margin = 20;
	vertices.reserve(numNodes);

	std::default_random_engine randEngine;
	std::uniform_real_distribution<float> xDis(static_cast<float>(0 + margin), static_cast<float>(windowWidth - margin));
	std::uniform_real_distribution<float> yDis(static_cast<float>(0 + margin), static_cast<float>(windowHeight - margin));
	std::uniform_int_distribution<> capacityDis(1, maxCapacity);
	randEngine.seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));

	// We re-generate nodes that are too close until we've reached the
	// maximum # of re-generation tries (so that we don't run in to an
	// infinite loop for some inputs)
	int tries = 0;
	static int maxTries = 10000;
	for (int i = 0; i < numNodes; )
	{
		float x = xDis(randEngine);
		float y = yDis(randEngine);
		sf::Vector2f pos(x, y);

		if (hasMinDistance(pos) || tries > maxTries)
		{
			vertices.push_back(Vertex(pos));
			++i;
		}
		else
		{
			++tries;
		}
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
			for (auto& edge : vert.edges)
			{
				if (GraphUtils::intersectWithWidth(vertices[pair.first].pos, vertices[pair.second].pos, vert.pos, vertices[edge.targetNode].pos, 0.f))
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

		vertices[pair.first].edges.emplace_back(pair.first, pair.second, capacityDis(randEngine));
		vertices[pair.second].edges.emplace_back(pair.second, pair.first, capacityDis(randEngine));

		++edgeCount;
	}

	selectStartAndTargetNodes();

	setBackwardEdgePointers();
}

void Graph::highlightPath(const std::deque<Edge*>& path)
{
	for (const auto& e : path)
	{
		// We are unnecessarily setting this flag on nodes multiple times,
		// but it doesn't really matter.
		vertices[e->startNode].renderInfo.isHighlighted = true;
		vertices[e->targetNode].renderInfo.isHighlighted = true;
		e->renderInfo.isHighlighted = true;
	}
}

void Graph::resetRenderInfo()
{
	for (auto& v : vertices)
	{
		v.renderInfo.reset();
		for (auto& e : v.edges)
		{
			e.renderInfo.reset();
		}
	}
}

void Graph::selectStartAndTargetNodes()
{
	start = 0;
	target = 0;

	std::random_device rd;
	auto rng = std::default_random_engine(rd());
	
	std::vector<size_t> nodes;
	nodes.reserve(size());
	for (size_t i = 1; i < size(); ++i)
	{
		nodes.push_back(i);
	}

	std::shuffle(nodes.begin(), nodes.end(), rng);

	for (const auto& i : nodes)
	{
		if (std::find_if(vertices[i].edges.begin(), vertices[i].edges.end(), [](const Edge& n) { return n.targetNode == 0; })
			== vertices[i].edges.end())
		{
			target = i;
			std::deque<Edge*> path;
			if (Algorithm::DFS(*this, path)) // any path from start to target?
			{
				break;
			}
		}
	}
}

void Graph::setBackwardEdgePointers()
{
	// I don't like this, but if we try and do this at the point
	// where we initially add the backward edges, we might invalidate the
	// pointer as more edges might be added to a vertex, so a pointer
	// pointing into the edges vector could be invalidated.

	size_t index = 0;
	for (auto& vert : vertices)
	{
		for (auto& edge : vert.edges)
		{
			for (auto& targetEdge : vertices[edge.targetNode].edges)
			{
				if (targetEdge.targetNode == index)
				{
					edge.setBackwardEdge(&targetEdge);
					break;
				}
			}
		}
		++index;
	}
}

bool Graph::hasMinDistance(const sf::Vector2f& p)
{
	const float minDistSquared = minDist * minDist;
	for (const auto& v : vertices)
	{
		sf::Vector2f diff = v.pos - p;

		if ((diff.x * diff.x + diff.y * diff.y) < minDistSquared)
		{
			return false;
		}
	}
	return true;
}
