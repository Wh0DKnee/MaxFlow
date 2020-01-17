#pragma once

#include <vector>
#include <random>
#include "Graph.h"

namespace GraphUtils
{
	float CCW(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c);

	bool intersect(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f d);

	bool intersectWithWidth(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f d, float width = 20.f);
};

