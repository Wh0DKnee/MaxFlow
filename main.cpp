#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "GraphUtils.h"
#include "DotWriter.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1600, 900), "");
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);

	char windowTitle[255] = "MaxFlow";
	window.setTitle(windowTitle);

	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;

	std::vector<Vertex> graph;
	char labelTags[255] = "[labeldistance=2 labelangle=0 headlabel=\"5/10\"]";
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

		ImGui::SFML::Update(window, deltaClock.restart());

		ImGui::Begin("Sample window"); // begin window
		//ImGui::Text(std::to_string(graph[0].posX).c_str());

		ImGui::InputInt("#nodes", &numNodes);
		
		ImGui::InputInt("totalCapacity", &totalCapacity);

		auto rerender = [&] {
			DotWriter::Write(graph, labelTags);
			// -Gsize=12,8\! -Gdpi=100
			system("cd graphviz & dot -Kneato -n -Tpng graph.dot -o renderedGraph.png");
			if (!image.loadFromFile("graphviz/renderedGraph.png"))
			{
				return -1;
			}

			texture.loadFromImage(image);  //Load Texture from image

			sprite.setTexture(texture);
		};

		if (ImGui::Button("re-draw")) 
		{
			system("cd graphviz & dot -Kneato -n -Tpng graph.dot -o renderedGraph.png");
			if (!image.loadFromFile("graphviz/renderedGraph.png"))
			{
				return -1;
			}

			texture.loadFromImage(image);  //Load Texture from image

			sprite.setTexture(texture);
		}

		if (ImGui::Button("re-generate"))
		{
			graph = GraphUtils::GenerateGraph(numNodes, totalCapacity);
			rerender();
		}

		if (ImGui::Button("re-render"))
		{ 
			rerender();
		}
		ImGui::InputText("edge label tags", labelTags, 255);

		ImGui::End(); // end window

		window.clear(sf::Color(209, 209, 209, 255));
		window.draw(sprite);
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
}