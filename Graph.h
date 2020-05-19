#pragma once
#include <vector>
#include <deque>
#include "Vertex.h"

//TODO: would be nice to be able to toggle between residual network and original network view.

class Graph
{
public:

	Graph() = default;

	// Randomly generates graph based on parameters.
	Graph(int numNodes, int maxCapacity, int windowWidth, int windowHeight, float minDist = 0.f);

	size_t getStart() const { return start; }
	size_t getTarget() const { return target; }

	void highlightPath(const std::deque<Edge*>& path);
	void resetRenderInfo();

	void setLevel(size_t index, int l);
	int getLevel(size_t index) const;

	void resetDinicInfo();

	// Proxy functions for vertices vector:

	// Returns # of vertices in this graph.
	size_t size() const { return vertices.size(); }

	Vertex& operator[](size_t i) { return vertices[i]; }
	const Vertex& operator[](size_t i) const { return vertices[i]; }

	// To enable range-based for loops:
	std::vector<Vertex>::iterator begin() { return vertices.begin(); }
	std::vector<Vertex>::const_iterator begin() const { return vertices.begin(); }
	std::vector<Vertex>::iterator end() { return vertices.end(); }
	std::vector<Vertex>::const_iterator end() const { return vertices.end(); }


private:
	std::vector<Vertex> vertices;
	size_t start = 0;
	size_t target = 0;
	float minDist = 0.f;

	// Dinic stuff
	std::vector<int> levels;

	void selectStartAndTargetNodes();

	void addBackwardEdges();

	void addCombinedEdges();

	bool hasMinDistance(const sf::Vector2f& p);
};

