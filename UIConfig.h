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

	static sf::Color regularColor = sf::Color::Black;
	static sf::Color highlightColor = sf::Color::Blue;

}