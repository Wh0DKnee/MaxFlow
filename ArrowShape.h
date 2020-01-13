#pragma once
#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/Vector2.hpp>

class ArrowShape : public sf::Shape
{
public:

	explicit ArrowShape(const sf::Vector2f& start, const sf::Vector2f& pointTo, float length = 10.f, float width = 5.f);

	virtual size_t getPointCount() const;

	virtual sf::Vector2f getPoint(size_t index) const;

private:
	sf::Vector2f dir;
	float length;
	float width;
};

