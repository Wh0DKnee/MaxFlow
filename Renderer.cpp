#include "Renderer.h"
#include "UIConfig.h"
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "LineShape.h"
#include "VectorUtils.h"
#include "ArrowShape.h"
#include <cmath>

#include "imgui.h"
#include "imgui-SFML.h"

using namespace UIConfig;

void Renderer::Render(sf::RenderWindow& window, const std::vector<Vertex>& graph, float deltaTime)
{
	sf::RectangleShape background = sf::RectangleShape(sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
	background.setFillColor(sf::Color(209, 209, 209, 255));
	window.draw(background);

	sf::Font font;
	if (!font.loadFromFile("fonts/arial.ttf"))
	{
		std::cout << "COULD NOT LOAD FONT, STOPPING RENDER" << std::endl;
	}

	arrowDistance += deltaTime * arrowSpeed;
	arrowDistance = std::fmod(arrowDistance, 1.f);
	
	if (ImGui::CollapsingHeader("Styling"))
	{
		ImGui::SliderFloat("node radius", &nodeRadius, 0.f, 30.f);
		ImGui::SliderFloat("label distance", &labelDistance, 0.f, 1.f);
		ImGui::SliderFloat("label spacing", &labelSpacing, 0.f, 100.f);
		ImGui::SliderFloat("label radius", &labelRadius, 0.f, 40.f);
		ImGui::SliderFloat("arrow distance", &arrowDistance, 0.f, 1.f);
		ImGui::SliderFloat("arrow size", &arrowLength, 0.f, 100.f);
		ImGui::SliderFloat("arrow speed", &arrowSpeed, 0.f, 3.f);
		ImGui::SliderInt("font size", &fontSize, 0, 30);
	}


	for (const auto& vert : graph)
	{
		sf::CircleShape nodeShape(nodeRadius);
		nodeShape.setFillColor(sf::Color::Black);
		nodeShape.setPosition(vert.pos.x, vert.pos.y);
		nodeShape.setOrigin(nodeRadius, nodeRadius);
		window.draw(nodeShape);
	}

	sf::Color regularColor = sf::Color(0, 0, 0, 255);
	std::vector<LineShape> edges;
	std::vector<sf::CircleShape> labelCircles;
	std::vector<sf::Text> labelTexts;
	std::vector<ArrowShape> arrows;

	size_t index = 0;
	for (const auto& vert : graph)
	{
		for (const auto& neighbor : vert.neighbors)
		{
			sf::Vector2f neighborPos = graph[neighbor.index].pos;
			sf::Vector2f delta = neighborPos - vert.pos;
			sf::Vector2f middle = vert.pos + (delta * labelDistance);
			sf::Vector2f perpendicular = sf::Vector2f(delta.y, -delta.x);
			sf::Vector2f labelPos = middle + VectorUtils::normalize(perpendicular) * labelSpacing;

			sf::CircleShape labelNode(labelRadius);
			labelNode.setOutlineThickness(2.f);
			labelNode.setOutlineColor(regularColor);
			labelNode.setPosition(labelPos);
			labelNode.setOrigin(labelRadius, labelRadius);
			labelCircles.push_back(labelNode);

			sf::Text labelText;
			labelText.setFont(font);
			labelText.setString("10");
			labelText.setCharacterSize(fontSize);
			labelText.setFillColor(sf::Color::Black);
			sf::FloatRect textRect = labelText.getLocalBounds();
			labelText.setOrigin(textRect.left + textRect.width / 2.f,
								textRect.top + textRect.height / 2.f);
			labelText.setPosition(labelPos);
			labelTexts.push_back(labelText);
			
			sf::Vector2f labelDelta = labelPos - vert.pos;
			sf::Vector2f labelDeltaNormalized = VectorUtils::normalize(labelDelta);
			LineShape line1(vert.pos + labelDeltaNormalized * nodeRadius, labelPos - labelDeltaNormalized * labelRadius);
			line1.setFillColor(regularColor);
			edges.push_back(line1);

			ArrowShape arrow1 = ArrowShape(vert.pos + labelDelta * arrowDistance, labelPos, arrowLength, arrowLength / 2.f);
			arrow1.setFillColor(regularColor);
			arrows.push_back(arrow1);

			labelDelta = neighborPos - labelPos;
			labelDeltaNormalized = VectorUtils::normalize(labelDelta);
			LineShape line2(labelPos + labelDeltaNormalized * labelRadius, neighborPos - labelDeltaNormalized * nodeRadius);
			line2.setFillColor(regularColor);
			edges.push_back(line2);

			ArrowShape arrow2 = ArrowShape(labelPos + labelDelta * arrowDistance, neighborPos, arrowLength, arrowLength / 2.f);
			arrow2.setFillColor(regularColor);
			arrows.push_back(arrow2);
		}
		++index;
	}

	// SFML doesnt have z-ordering, so we draw edges first...
	for (const auto& drawable : edges)
	{
		window.draw(drawable);
	}

	for (const auto& drawable : arrows)
	{
		window.draw(drawable);
	}

	// ... and labels second, so that edges dont hide the labels.
	for (const auto& drawable : labelCircles)
	{
		window.draw(drawable);
	}

	for (const auto& drawable : labelTexts)
	{
		window.draw(drawable);
	}
}
