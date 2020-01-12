#include "Renderer.h"
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "LineShape.h"
#include <cmath>

#include "imgui.h"
#include "imgui-SFML.h"

sf::Vector2f normalize(const sf::Vector2f& v)
{
	return v / std::sqrt(v.x * v.x + v.y * v.y);
}

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
	static float labelRadius = 15.f;
	static int fontSize = 15;
	
	if (ImGui::CollapsingHeader("Styling"))
	{
		ImGui::SliderFloat("node radius", &nodeRadius, 1.f, 30.f);
		ImGui::SliderFloat("label distance", &labelDistance, 0.f, 1.f);
		ImGui::SliderFloat("label spacing", &labelSpacing, 10.f, 100.f);
		ImGui::SliderFloat("label radius", &labelRadius, 10.f, 40.f);
		ImGui::SliderInt("font size", &fontSize, 5, 30);
	}

	for (const auto& vert : graph)
	{
		sf::CircleShape nodeShape(nodeRadius);
		nodeShape.setFillColor(sf::Color::Black);
		nodeShape.setPosition(vert.pos.x, vert.pos.y);
		nodeShape.setOrigin(nodeRadius, nodeRadius);
		window.draw(nodeShape);
	}

	size_t index = 0;
	for (const auto& vert : graph)
	{
		for (const auto& neighbor : vert.neighbors)
		{
			sf::Vector2f neighborPos = graph[neighbor.index].pos;
			sf::Vector2f delta = neighborPos - vert.pos;
			sf::Vector2f middle = vert.pos + (delta * labelDistance);
			sf::Vector2f perpendicular = sf::Vector2f(delta.y, -delta.x);
			sf::Vector2f labelPos = middle + normalize(perpendicular) * labelSpacing;

			sf::CircleShape labelNode(labelRadius);
			labelNode.setOutlineThickness(2.f);
			labelNode.setOutlineColor(sf::Color::Black);

			labelNode.setPosition(labelPos);
			labelNode.setOrigin(labelRadius, labelRadius);
			window.draw(labelNode);

			sf::Text labelText;
			labelText.setFont(font);
			labelText.setString("5/10");
			labelText.setCharacterSize(fontSize);
			labelText.setFillColor(sf::Color::Black);
			sf::FloatRect textRect = labelText.getLocalBounds();
			labelText.setOrigin(textRect.left + textRect.width / 2.f,
				textRect.top + textRect.height / 2.f);
			labelText.setPosition(labelPos);
			window.draw(labelText);
			
			sf::Vector2f labelDeltaNormalized = normalize(labelPos - vert.pos);
			LineShape line1(vert.pos + labelDeltaNormalized * nodeRadius, labelPos - labelDeltaNormalized * labelRadius);
			line1.setFillColor(sf::Color::Black);
			window.draw(line1);

			labelDeltaNormalized = normalize(neighborPos - labelPos);
			LineShape line2(labelPos + labelDeltaNormalized * labelRadius, neighborPos - labelDeltaNormalized * nodeRadius);
			line2.setFillColor(sf::Color::Black);
			window.draw(line2);
		}
		++index;
	}
}
