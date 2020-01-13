#include "ArrowShape.h"
#include "VectorUtils.h"
#include <iostream>

ArrowShape::ArrowShape(const sf::Vector2f& start, const sf::Vector2f& pointTo, float length_, float width_)
	: length(length_), width(width_)
{
	setPosition(start);
	dir = VectorUtils::normalize(pointTo - start);
	update();
}

size_t ArrowShape::getPointCount() const
{
	return 3;
}

sf::Vector2f ArrowShape::getPoint(size_t index) const
{
	sf::Vector2f unitPerpendicular(dir.y, -dir.x);
	
	switch (index)
	{
	case 0:		return sf::Vector2f(0.f, 0.f);
	case 1:		return (dir * length);
	case 2:		return (unitPerpendicular * width);
	default:	return sf::Vector2f();
	}
}
