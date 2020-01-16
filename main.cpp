#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "GraphUtils.h"
#include "Renderer.h"

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(1600, 900), "MaxFlow", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);

	std::vector<Vertex> graph;
	int numNodes = 10;
	int totalCapacity = 100;

	window.resetGLStates();
	sf::Clock deltaClock;
	while (window.isOpen()) { // main loop
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		sf::Time deltaTime = deltaClock.restart();
		ImGui::SFML::Update(window, deltaTime);

		ImGui::Begin("Debug"); // begin window
		//ImGui::Text("FPS: %i", static_cast<int>(1.f / deltaTime.asSeconds()));
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::InputInt("#nodes", &numNodes);
		ImGui::InputInt("totalCapacity", &totalCapacity);

		if (ImGui::Button("re-generate"))
		{
			graph = GraphUtils::generateGraph(numNodes, totalCapacity, window.getSize().x, window.getSize().y);
		}
		
		window.clear(sf::Color(209, 209, 209, 255));
		Renderer::Render(window, graph, deltaTime.asSeconds());
		ImGui::End(); // end window
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
}