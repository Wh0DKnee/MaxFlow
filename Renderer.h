#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include "Vertex.h"

namespace sf {
	class RenderWindow;
}
// Not sure if this will need state at some point, so I made it a class instead of a namespace.
class Renderer
{
public:
	static void render(class sf::RenderWindow& window, const std::vector<Vertex>& graph, float deltaTime);
};

