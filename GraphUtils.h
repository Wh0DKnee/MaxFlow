#pragma once

#include <vector>
#include <random>
#include "Vertex.h"

namespace GraphUtils
{
	float CCW(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c);

	bool intersect(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f d);

	bool intersectWithWidth(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f d, float width = 20.f);

	std::vector<Vertex> generateGraph(int numNodes, int maxCapacity, int windowWidth, int windowHeight);

	void setCapacitiesRandomly(std::vector<Vertex>& graph, long long edgeCount, int maxCapacity);
};

