#include "LineShape.h"
#include <cmath>

LineShape::LineShape(const sf::Vector2f& point1, const sf::Vector2f& point2) :
	m_direction(point2 - point1)
{
	setPosition(point1);
	setThickness(2.f);
}


void LineShape::setThickness(float thickness)
{
	m_thickness = thickness;
	update();
}


float LineShape::getThickness() const
{
	return m_thickness;
}


float LineShape::getLength() const
{
	return std::sqrt(m_direction.x * m_direction.x + m_direction.y * m_direction.y);
}


size_t LineShape::getPointCount() const
{
	return 4;
}


sf::Vector2f LineShape::getPoint(size_t index) const
{
	sf::Vector2f unitDirection = m_direction / getLength();
	sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

	sf::Vector2f offset = (m_thickness / 2.f) * unitPerpendicular;

	switch (index)
	{
	default:
	case 0: return offset;
	case 1: return (m_direction + offset);
	case 2: return (m_direction - offset);
	case 3: return (-offset);
	}
}
