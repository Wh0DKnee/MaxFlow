#pragma once
#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/Vector2.hpp>

class LineShape : public sf::Shape
{
public:

	explicit LineShape(const sf::Vector2f& point1, const sf::Vector2f& point2);
	
	void setThickness(float thickness);

	float getThickness() const;

	float getLength() const;

	virtual size_t getPointCount() const;

	virtual sf::Vector2f getPoint(size_t index) const;

private:

	sf::Vector2f m_direction; ///< Direction of the line
	float m_thickness;    ///< Thickness of the line
};

