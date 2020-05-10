#pragma once
#include <SFML/Graphics/Color.hpp>

namespace UIConfig
{
	static float nodeRadius = 10.f;
	static float labelDistance = 0.5f;
	static float labelSpacing = 9.5f;
	static float labelRadius = 6.8f;
	static float arrowDistance = 0.5f;
	static float arrowLength = 10.f;
	static float arrowSpeed = 0.3f;
	static const char* fonts[] = { "arial", "times", "segoeui", "calibri" };
	static const char* font = "segoeui";
	static int fontSize = 12;
	static bool drawNodeLabels = true;

	static sf::Color startColor = sf::Color(45, 102, 43);
	static sf::Color targetColor = sf::Color(112, 31, 31);

}