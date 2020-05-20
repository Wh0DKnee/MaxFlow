#pragma once
#include <string>
#include <SFML/Graphics/Color.hpp>
#include "UIConfig.h"

struct RenderInfo
{
	void highlight();

	void resetHighlight();

	sf::Color getColor() const { return color; }
	void setColor(sf::Color c) { color = c; }

	sf::Color getRegularColor() const { return regularColor; }
	void setRegularColor(sf::Color c);
	sf::Color getHighlightColor() const { return highlightColor; }
	void setHighlightColor(sf::Color c);

private:

	bool isHighlighted = false; // TODO: need this?
	sf::Color regularColor = sf::Color::Black;
	sf::Color highlightColor = sf::Color::Blue;
	sf::Color color = regularColor;
};

struct Vertex;

struct VertexRenderInfo : public RenderInfo
{
	void reset();

	std::string getLabel(const Vertex& vert) const;

	enum LabelType { NONE, HEIGHT };
	LabelType labelType = LabelType::NONE;
	float radius = UIConfig::nodeRadius;
};

