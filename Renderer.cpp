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
#include <algorithm>
#include "Graph.h"

#include "imgui.h"
#include "imgui-SFML.h"

namespace UI = UIConfig; // alias

void Renderer::render(sf::RenderWindow& window, const Graph& graph, float deltaTime)
{
	sf::RectangleShape background = sf::RectangleShape(sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
	background.setFillColor(sf::Color(209, 209, 209, 255));
	window.draw(background);

	renderImGUI();

	UI::arrowDistance += deltaTime * UI::arrowSpeed;
	UI::arrowDistance = std::fmod(UI::arrowDistance, 1.f);
	
	sf::Font font;
	std::string fontPath = "fonts/" + std::string(UI::font) + ".ttf";
	if (!font.loadFromFile(fontPath))
	{
		std::cout << "COULD NOT LOAD FONT, STOPPING RENDER" << std::endl;
		return;
	}

	for (const auto& vert : graph)
	{
		if (vert.renderInfo.shape == VertexRenderInfo::Shape::CIRCLE)
		{
			sf::CircleShape nodeShape(vert.renderInfo.radius);
			nodeShape.setFillColor(vert.renderInfo.getColor());
			nodeShape.setPosition(vert.pos.x, vert.pos.y);
			nodeShape.setOrigin(vert.renderInfo.radius, vert.renderInfo.radius);
			window.draw(nodeShape);
		}
		else if (vert.renderInfo.shape == VertexRenderInfo::Shape::RECT)
		{
			sf::RectangleShape nodeShape(sf::Vector2f(vert.renderInfo.squareLength, vert.renderInfo.squareLength));
			nodeShape.setFillColor(vert.renderInfo.getColor());
			nodeShape.setPosition(vert.pos.x, vert.pos.y);
			nodeShape.setOrigin(vert.renderInfo.squareLength / 2.f, vert.renderInfo.squareLength / 2.f);
			window.draw(nodeShape);
		}

	}

	if (UI::drawNodeLabels)
	{
		size_t index = 0;
		for (const auto& vert : graph)
		{
			sf::Text nodeText;
			nodeText.setFont(font);
			//nodeText.setString(std::to_string(index));
			//nodeText.setString(std::to_string(graph.getLevel(index)));
			nodeText.setString(vert.renderInfo.getLabel(vert));
			nodeText.setCharacterSize(static_cast<unsigned int>(vert.renderInfo.radius * 2.f));
			nodeText.setFillColor(sf::Color::White);
			sf::FloatRect textRect = nodeText.getLocalBounds();
			nodeText.setOrigin(textRect.left + textRect.width / 2.f,
				textRect.top + textRect.height / 2.f);
			nodeText.setPosition(vert.pos.x, vert.pos.y);
			window.draw(nodeText);
			++index;
		}
	}

	std::vector<LineShape> edges;
	std::vector<sf::CircleShape> labelCircles;
	std::vector<sf::Text> labelTexts;
	std::vector<ArrowShape> arrows;

	size_t index = 0;
	for (const auto& vert : graph)
	{
		for (const auto& edge : vert.renderedEdges)
		{
			sf::Vector2f neighborPos = graph[edge.targetNode].pos;
			sf::Vector2f delta = neighborPos - vert.pos;
			sf::Vector2f middle = vert.pos + (delta * UI::labelDistance);
			sf::Vector2f perpendicular = sf::Vector2f(delta.y, -delta.x);
			sf::Vector2f labelPos = middle + VectorUtils::normalize(perpendicular) * UI::labelSpacing;

			sf::CircleShape labelNode(UI::labelRadius);
			labelNode.setOutlineThickness(2.f);
			labelNode.setOutlineColor(sf::Color::Black);
			labelNode.setPosition(labelPos);
			labelNode.setOrigin(UI::labelRadius, UI::labelRadius);
			labelCircles.push_back(labelNode);

			sf::Text labelText;
			labelText.setFont(font);
			labelText.setStyle(sf::Text::Bold);
			labelText.setString(std::to_string(edge.getResidualCapacity()));
			labelText.setCharacterSize(UI::fontSize);
			labelText.setFillColor(sf::Color::Black);
			sf::FloatRect textRect = labelText.getLocalBounds();
			labelText.setOrigin(textRect.left + textRect.width / 2.f,
								textRect.top + textRect.height / 2.f);
			labelText.setPosition(labelPos);
			labelTexts.push_back(labelText);
			
			sf::Vector2f labelDelta = labelPos - vert.pos;
			sf::Vector2f labelDeltaNormalized = VectorUtils::normalize(labelDelta);
			LineShape line1(vert.pos + labelDeltaNormalized * vert.renderInfo.radius, labelPos - labelDeltaNormalized * UI::labelRadius);
			line1.setFillColor(edge.renderInfo.getColor());
			edges.push_back(line1);

			float arrowLength = UI::arrowLength + (std::min(edge.renderedFlow / static_cast<float>(graph.getMaxCapacity()), 1.f) * UI::maxAdditionalArrowLength);
			float arrowWidth = arrowLength / 2.f;

			ArrowShape arrow1 = ArrowShape(vert.pos + labelDelta * UI::arrowDistance, labelPos, arrowLength, arrowWidth);
			arrow1.setFillColor(edge.renderInfo.getColor());
			arrows.push_back(arrow1);

			labelDelta = neighborPos - labelPos;
			labelDeltaNormalized = VectorUtils::normalize(labelDelta);
			LineShape line2(labelPos + labelDeltaNormalized * UI::labelRadius, neighborPos - labelDeltaNormalized * vert.renderInfo.radius);
			line2.setFillColor(edge.renderInfo.getColor());
			edges.push_back(line2);

			ArrowShape arrow2 = ArrowShape(labelPos + labelDelta * UI::arrowDistance, neighborPos, arrowLength, arrowWidth);
			arrow2.setFillColor(edge.renderInfo.getColor());
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

void Renderer::renderImGUI()
{
	ImGui::Begin("Styling");

	ImGui::SliderFloat("node radius", &UI::nodeRadius, 0.f, 30.f);
	ImGui::SliderFloat("label distance", &UI::labelDistance, 0.f, 1.f);
	ImGui::SliderFloat("label spacing", &UI::labelSpacing, 0.f, 100.f);
	ImGui::SliderFloat("label radius", &UI::labelRadius, 0.f, 40.f);
	ImGui::SliderFloat("arrow distance", &UI::arrowDistance, 0.f, 1.f);
	ImGui::SliderFloat("arrow size", &UI::arrowLength, 0.f, 100.f);
	ImGui::SliderFloat("max additional arrow size", &UI::maxAdditionalArrowLength, 0.f, 40.f);
	ImGui::SliderFloat("arrow speed", &UI::arrowSpeed, 0.f, 3.f);
	ImGui::SliderInt("font size", &UI::fontSize, 0, 30);
	ImGui::Checkbox("node labels", &UI::drawNodeLabels);

	if (ImGui::BeginCombo("font", UI::font))
	{
		for (int n = 0; n < IM_ARRAYSIZE(UI::fonts); n++)
		{
			bool is_selected = (UI::font == UI::fonts[n]);
			if (ImGui::Selectable(UI::fonts[n], is_selected))
				UI::font = UI::fonts[n];
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGui::End();
}
