#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "GraphUtils.h"
#include "DotWriter.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1600, 1000), "");
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);

	char windowTitle[255] = "MaxFlow";
	window.setTitle(windowTitle);

	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;

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

		ImGui::SFML::Update(window, deltaClock.restart());

		ImGui::Begin("Sample window"); // begin window
		//ImGui::Text(std::to_string(graph[0].posX).c_str());

		if (ImGui::Button("re-render"))
		{ 
			auto graph = GraphUtils::GenerateGraph(10, 24);
			DotWriter::Write(graph);
			system("cd graphviz & dot -Kfdp -n -Gsize=12,8\! -Gdpi=100 -Tpng graph.dot -o renderedGraph.png");
			if (!image.loadFromFile("graphviz/renderedGraph.png"))
			{
				return -1;
			}

			texture.loadFromImage(image);  //Load Texture from image

			sprite.setTexture(texture);
		}

		ImGui::End(); // end window

		window.clear(sf::Color(209, 209, 209, 255));
		window.draw(sprite);
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
}