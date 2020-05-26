#pragma once
#include "Visualizer.h"
#include <SFML/Graphics/Color.hpp>

class PushRelabelVis : public Visualizer
{
public:
	virtual void reset() override;

	virtual void step() override;

	virtual void runAlgorithm() override;

	static const float nodeRenderRadius;

private:
	bool isFirstStep = true;

	void saturatedCutVis();

	size_t active = 0;
	size_t previousActive = 0;
	sf::Color activeColor = sf::Color(255, 136, 0);
};

bool colorEqualIgnoreAlpha(const sf::Color& lhs, const sf::Color& rhs);

