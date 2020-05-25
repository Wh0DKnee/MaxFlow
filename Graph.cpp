#include "Graph.h"
#include "GraphUtils.h"
#include <chrono>
#include <cassert>
#include <limits>
#include "Algorithm.h"
#include "UIConfig.h"

Graph::Graph(int numNodes, int maxCap, int windowWidth, int windowHeight, float minDist, unsigned int seed /*= std::numeric_limits<unsigned int>::max()*/) 
	: minDist(minDist), maxCapacity(maxCap), numNodes(numNodes), windowWidth(windowWidth), windowHeight(windowHeight), seed(seed)
{
	if (seed == std::numeric_limits<unsigned int>::max())
	{
		this->seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
	}

	Init();
}

void Graph::Init()
{
	static int margin = 20;
	vertices.reserve(numNodes);

	std::default_random_engine randEngine;
	std::uniform_real_distribution<float> xDis(static_cast<float>(0 + margin), static_cast<float>(windowWidth - margin));
	std::uniform_real_distribution<float> yDis(static_cast<float>(0 + margin), static_cast<float>(windowHeight - margin));
	std::uniform_int_distribution<> capacityDis(1, maxCapacity);
	randEngine.seed(seed);

	// We re-generate nodes that are too close (for visualization purposes) until we've
	// reached the maximum # of re-generation tries (so that we don't run into an
	// infinite loop for some inputs)
	// TODO: Use a more sophisticated approach, for example https://en.wikipedia.org/wiki/Lloyd's_algorithm
	int tries = 0;
	static int maxTries = 10000;
	for (int i = 0; i < numNodes; )
	{
		float x = xDis(randEngine);
		float y = yDis(randEngine);
		sf::Vector2f pos(x, y);

		if (hasMinDistance(pos) || tries > maxTries)
		{
			vertices.emplace_back(pos);
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

		// add two edges, one in each direction
		vertices[pair.first].edges.emplace_back(pair.first, pair.second, capacityDis(randEngine), true);
		vertices[pair.second].edges.emplace_back(pair.second, pair.first, capacityDis(randEngine), true);
	}

	// TODO: Add option to generate graph without backward edges in original graph.
	addBackwardEdges();

	addCombinedEdges();

	resetDinicLevels();

	selectSourceAndSinkNodes();

	increaseStartAndTargetIncidentCapacities();

	initializeHeights();
}

void Graph::reset()
{
	vertices.clear();
	levels.clear();
	Init();
}

int Graph::getFlow() const
{
	return vertices[getSource()].getOutGoingFlow() - vertices[getSource()].getIncomingFlow();
}

void Graph::highlightPath(const std::deque<Edge*>& path)
{
	for (const auto& e : path)
	{
		// We are unnecessarily setting this flag on nodes multiple times,
		// but it doesn't really matter.
		vertices[e->startNode].renderInfo.highlight();
		vertices[e->targetNode].renderInfo.highlight();
		e->highlight();
	}
}

void Graph::resetRenderInfo()
{
	for (auto& v : vertices)
	{
		v.renderInfo.reset();
		for (auto& e : v.edges)
		{
			e.resetHighlight();
		}
	}
}

void Graph::setLevel(size_t index, int l)
{
	levels[index] = l;
}

int Graph::getLevel(size_t index) const
{
	return levels[index];
}

void Graph::resetDinicLevels()
{
	levels.assign(size(), -1);
}

void Graph::selectSourceAndSinkNodes()
{
	source = 0;
	sink = std::numeric_limits<size_t>::max(); // intentionally setting this to an invalid value here

	// BFS sets the levels for dinic, we abuse this here to find the
	// node with max distance from the source node.
	std::deque<Edge*> path;
	auto searchResult = Algorithm::BFS(*this, path); 

	int maxLevel = -1;
	size_t maxLevelIndex = 0;
	size_t index = 0;
	for (const auto& l : levels)
	{
		if (l > maxLevel)
		{
			maxLevel = l;
			maxLevelIndex = index;
		}
		++index;
	}
	
	sink = maxLevelIndex;

	vertices[source].renderInfo.setRegularColor(UIConfig::startColor);
	vertices[source].renderInfo.setHighlightColor(UIConfig::startColor);
	vertices[sink].renderInfo.setRegularColor(UIConfig::targetColor);
	vertices[sink].renderInfo.setHighlightColor(UIConfig::targetColor);
}

void Graph::addBackwardEdges()
{
	std::vector<size_t> originalNumEdges;

	size_t index = 0;
	for (auto& vert : vertices)
	{
		// Allocate memory for back edges, so that no resizing
		// happens when we add them in the for loop below. This way,
		// we can safely store pointers into the edges vector,
		// as this will be its final size for the rest of its
		// life time.
		// Should this ever change, change the edge vector in
		// Vertex to a list!!!
		// In this process, all vertices will double its # of
		// edges, because for every edge, there is a reverse edge
		// in the original graph, so #edges in = #edges out
		vert.edges.reserve(2 * vert.edges.size());

		// we only want to add backward edges for the original edges
		// (read: we don't want additional backward edges for backward edges)
		originalNumEdges.push_back(vert.edges.size());
		++index;
	}

	index = 0;
	for (auto& vert : vertices)
	{

		for (size_t i = 0; i < originalNumEdges[index]; ++i)
		{
			auto& curEdge = vert.edges[i];
			auto& targetVertEdges = vertices[curEdge.targetNode].edges;

			// backward edge starts with residual capacity of 0.
			Edge e(curEdge.targetNode, curEdge.startNode, 0);
			targetVertEdges.push_back(e);
			curEdge.setBackwardEdge(&targetVertEdges[targetVertEdges.size() - 1]);
			targetVertEdges[targetVertEdges.size() - 1].setBackwardEdge(&curEdge);
		}

		++index;
	}

}

void Graph::addCombinedEdges()
{
	for (auto& v : vertices)
	{
		for (size_t i = 0; i < v.edges.size(); ++i)
		{
			size_t targetNode = v.edges[i].targetNode;
			
			for (size_t j = i + 1; j < v.edges.size(); ++j)
			{
				if (v.edges[j].targetNode == targetNode)
				{
					Edge e(	v.edges[i].startNode,
							v.edges[i].targetNode,
							v.edges[i].getResidualCapacity() + v.edges[j].getResidualCapacity());

					v.renderedEdges.push_back(e);
					v.edges[i].setCombinedEdge(&v.renderedEdges.back());
					v.edges[j].setCombinedEdge(&v.renderedEdges.back());
				}
			}
		}
	}
}

void Graph::increaseStartAndTargetIncidentCapacities()
{
	for (auto& e : vertices[getSource()].edges)
	{
		if (e.isOriginal)
		{
			e.addCapacity(5*maxCapacity);
		}
	}

	for (auto& e : vertices[getSink()].edges)
	{
		if (!e.isOriginal)
		{
			assert(e.getBackwardEdge() != nullptr);
			e.getBackwardEdge()->addCapacity(5*maxCapacity);
		}
	}
}

void Graph::initializeHeights()
{
	// all nodes are initialized with height zero by default
	// but the source height is set to |V|
	vertices[getSource()].setHeight((int)size());

	// Alternatively, we could do a reverse BFS from the source to compute exact height
	// labels, which is preferable in practice, but doesn't change the asymptotic complexity.
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
