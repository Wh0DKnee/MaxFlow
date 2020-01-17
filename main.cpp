#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "GraphUtils.h"
#include "Renderer.h"
#include "Algorithm.h"
#include "Graph.h"

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(1600, 900), "MaxFlow", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);

	Graph graph;
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
			graph = Graph(numNodes, totalCapacity, window.getSize().x, window.getSize().y);
		}

		if (ImGui::Button("DFS"))
		{
			auto path = Algorithm::DFS(graph, 0, 1);
			size_t index = 0;
			for (const auto& v : path)
			{
				graph[v].renderInfo.isHighlighted = true;
				if (index < path.size() - 1)
				{
					graph[v].renderInfo.successor = path[index + 1];
				}

				++index;
			}
		}
		
		window.clear(sf::Color(209, 209, 209, 255));
		Renderer::render(window, graph, deltaTime.asSeconds());
		ImGui::End(); // end window
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
}