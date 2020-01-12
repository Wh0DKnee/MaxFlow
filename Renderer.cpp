#include "Renderer.h"
#include <string>
#include <iomanip>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "LineShape.h"

void Renderer::Render(sf::RenderWindow& window, const std::vector<Vertex>& graph)
{
	sf::Font font;
	if (!font.loadFromFile("fonts/arial.ttf"))
	{
		std::cout << "COULD NOT LOAD FONT, STOPPING RENDER" << std::endl;
	}

	static float nodeRadius = 10.f;
	static float labelDistance = 0.5f;
	static float labelSpacing = 20.f;
	static float labelRadius = 9.f;

	for (const auto& vert : graph)
	{
		sf::CircleShape nodeShape(nodeRadius);
		nodeShape.setFillColor(sf::Color::Black);
		nodeShape.setPosition(vert.pos.x, vert.pos.y);
		window.draw(nodeShape);
	}

	size_t index = 0;
	for (const auto& vert : graph)
	{
		for (const auto& neighbor : vert.neighbors)
		{
			Point delta = graph[neighbor.index].pos - vert.pos;
			Point middle = vert.pos + (delta * labelDistance);
			Point perpendicular = Point(delta.y, -delta.x);
			Point pos = middle + perpendicular.normalize() * labelSpacing;

			sf::CircleShape labelNode(labelRadius);
			labelNode.setOutlineThickness(2.f);
			labelNode.setOutlineColor(sf::Color::Black);

			labelNode.setPosition(pos.x, pos.y);
			window.draw(labelNode);

			sf::Text labelText;
			labelText.setFont(font);
			labelText.setString("5/10");
			labelText.setCharacterSize(15);
			labelText.setFillColor(sf::Color::Black);
			labelText.setPosition(pos.x, pos.y);
			window.draw(labelText);
			
			LineShape line1(sf::Vector2f(vert.pos.x, vert.pos.y), sf::Vector2f(pos.x, pos.y));
		}
		++index;
	}
}
