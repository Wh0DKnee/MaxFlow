#pragma once
#include <vector>
#include "Vertex.h"

class Graph
{
public:

	Graph() = default;

	// Randomly generates graph based on parameters.
	Graph(int numNodes, int maxCapacity, int windowWidth, int windowHeight);

	size_t getStart() const { return start; }
	size_t getTarget() const { return target; }

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
	size_t start;
	size_t target;

	void setCapacitiesRandomly(long long edgeCount, int maxCapacity);

	void selectStartAndTargetNodes();
};

