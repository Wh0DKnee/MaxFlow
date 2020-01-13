#include "VectorUtils.h"
#include <cmath>

sf::Vector2f VectorUtils::normalize(const sf::Vector2f& v)
{
	return v / std::sqrt(v.x * v.x + v.y * v.y);
}
