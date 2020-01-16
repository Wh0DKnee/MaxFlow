#pragma once

#include <SFML/System/Vector2.hpp>

namespace VectorUtils
{
	sf::Vector2f normalize(const sf::Vector2f& v);

	float dot(const sf::Vector2f& v1, const sf::Vector2f& v2);
}

