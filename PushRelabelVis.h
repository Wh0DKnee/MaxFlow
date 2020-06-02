#pragma once
#include "Visualizer.h"
#include <SFML/Graphics/Color.hpp>

class PushRelabelVis : public Visualizer
{
public:
	virtual void reset() override;

	virtual void step() override;

	virtual void runAlgorithm() override;

	// Push Relabel has height labels on nodes, so we make them bigger for readability.
	static const float nodeRenderRadius;
	static const float nodeSquareRenderLength;

	void saturatedCutVis();

private:
	bool isFirstStep = true;

	size_t active = 0;
	size_t previousActive = 0;
	sf::Color activeColor = sf::Color(255, 136, 0);
};

bool colorEqualIgnoreAlpha(const sf::Color& lhs, const sf::Color& rhs);

