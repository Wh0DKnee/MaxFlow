#include "VectorUtils.h"
#include <cmath>

sf::Vector2f VectorUtils::normalize(const sf::Vector2f& v)
{
	return v / std::sqrt(v.x * v.x + v.y * v.y);
}

float VectorUtils::dot(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}
