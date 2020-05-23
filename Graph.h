#pragma once
#include <vector>
#include <deque>
#include <queue>
#include "Vertex.h"

//TODO: would be nice to be able to toggle between residual network and original network view.

class Graph
{
public:

	Graph() = default;

	// Randomly generates graph based on parameters.
	Graph(int numNodes, int maxCap, int windowWidth, int windowHeight, float minDist = 0.f);

	int maxCapacity;

	size_t getStart() const { return start; }
	size_t getTarget() const { return target; }

	void highlightPath(const std::deque<Edge*>& path);
	void resetRenderInfo();

	void setLevel(size_t index, int l);
	int getLevel(size_t index) const;

	void resetDinicLevels();

	// TODO: this probably shouldn't be public - befriend all push relabel functions?
	// But then they would have access to all private members...

	// Only stores which vertices have excess, the actual excess
	// value is stored on the vertex itself. Use like: graph[verticesWithExcess.front()].getExcess()
	std::queue<size_t> verticesWithExcess;

	// Proxy functions for vertices vector:
#pragma region proxy_functions
	// Returns # of vertices in this graph.
	size_t size() const { return vertices.size(); }

	Vertex& operator[](size_t i) { return vertices[i]; }
	const Vertex& operator[](size_t i) const { return vertices[i]; }

	// To enable range-based for loops:
	std::vector<Vertex>::iterator begin() { return vertices.begin(); }
	std::vector<Vertex>::const_iterator begin() const { return vertices.begin(); }
	std::vector<Vertex>::iterator end() { return vertices.end(); }
	std::vector<Vertex>::const_iterator end() const { return vertices.end(); }
#pragma endregion proxy_functions

private:
	std::vector<Vertex> vertices;
	size_t start = 0;
	size_t target = 0;
	float minDist = 0.f;

	// for Dinic - we store this here instead of on the vertices
	// because resetting can be done in constant time this way.
	// Otherwise we'd have to iterate the vertices and hence linear time.
	// Heights for push relabel are stored on vertices though, as they
	// don't have to be reset and it makes the code cleaner.
	std::vector<int> levels;

	void selectStartAndTargetNodes();

	void addBackwardEdges();

	void addCombinedEdges();

	// for push relabel
	void initializeHeights();

	bool hasMinDistance(const sf::Vector2f& p);
};

