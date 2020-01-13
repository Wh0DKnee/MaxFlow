#include "LineShape.h"
#include <cmath>

LineShape::LineShape(const sf::Vector2f& point1, const sf::Vector2f& point2) :
	dir(point2 - point1)
{
	setPosition(point1);
	setThickness(2.f);
}


void LineShape::setThickness(float thickness_)
{
	thickness = thickness_;
	update();
}


float LineShape::getThickness() const
{
	return thickness;
}


float LineShape::getLength() const
{
	return std::sqrt(dir.x * dir.x + dir.y * dir.y);
}


size_t LineShape::getPointCount() const
{
	return 4;
}


sf::Vector2f LineShape::getPoint(size_t index) const
{
	sf::Vector2f unitDirection = dir / getLength();
	sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

	sf::Vector2f offset = (thickness / 2.f) * unitPerpendicular;

	switch (index)
	{
	default:
	case 0: return offset;
	case 1: return (dir + offset);
	case 2: return (dir - offset);
	case 3: return (-offset);
	}
}
