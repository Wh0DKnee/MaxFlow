#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "GraphUtils.h"
#include "Renderer.h"
#include "Algorithm.h"
#include "Graph.h"
#include "SinglePathFlowVis.h"
#include "DinicVisualizer.h"
#include "PushRelabelVis.h"
#include "TestEnvironment.h"

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(1600, 900), "MaxFlow", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);

	Graph graph(10, 10, window.getSize().x, window.getSize().y, 150.f);

	std::vector<Visualizer*> visualizers;
	SinglePathFlowVis singlePathFlowVis; // Ford Fulkerson and Edmonds Karp
	singlePathFlowVis.setGraph(graph);
	DinicVisualizer dinicVis;
	dinicVis.setGraph(graph);
	PushRelabelVis pushRelabelVis;
	pushRelabelVis.setGraph(graph);

	visualizers.push_back(&singlePathFlowVis);
	visualizers.push_back(&dinicVis);
	visualizers.push_back(&pushRelabelVis);
	Visualizer* currentVisualizer = &singlePathFlowVis; // default

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

		ImGui::SetNextWindowBgAlpha(0.8f);
		ImGui::Begin("Debug"); // begin window
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		static int numNodes = 10;
		ImGui::InputInt("#nodes", &numNodes);
		static int maxCapacity = 10;
		ImGui::InputInt("totalCapacity", &maxCapacity);
		static float minDistance = 150.f;
		ImGui::InputFloat("minDistance", &minDistance);
		static int seed = 0;
		ImGui::InputInt("custom seed", &seed);
		static bool useCustomSeed = false;
		ImGui::Checkbox("use custom seed", &useCustomSeed);

		// A bit of ugly UI code - okay for now though (AKA I'll never touch this again)
		if (ImGui::Button("re-generate"))
		{
			assert(numNodes >= 0 && maxCapacity >= 0);
			unsigned int s = useCustomSeed ? static_cast<unsigned int>(seed) : std::numeric_limits<unsigned int>::max();
			
			graph = Graph(numNodes, maxCapacity, window.getSize().x, window.getSize().y, minDistance, s);

			for (auto& v : visualizers)
			{
				v->setGraph(graph);
				v->reset();
			}
		}
		if (ImGui::Button("reset"))
		{
			graph.reset();
		}

		if (ImGui::CollapsingHeader("Algo"))
		{
			static std::string currentAlgorithmText = "Choose Algorithm";

			if (ImGui::Button("FordFulkerson"))
			{
				singlePathFlowVis.reset();
				singlePathFlowVis.setAlgo(Algorithm::AlgoType::FORD_FULKERSON);
				currentVisualizer = &singlePathFlowVis;
				currentAlgorithmText = "Ford Fulkerson";
			}
			ImGui::SameLine();

			if (ImGui::Button("Edmonds Karp"))
			{
				singlePathFlowVis.reset();
				singlePathFlowVis.setAlgo(Algorithm::AlgoType::EDMONDS_KARP);
				currentVisualizer = &singlePathFlowVis;
				currentAlgorithmText = "Edmonds Karp";
			}
			ImGui::SameLine();

			if (ImGui::Button("Dinic"))
			{
				dinicVis.reset();
				currentVisualizer = &dinicVis;
				currentAlgorithmText = "Dinic";
			}
			ImGui::SameLine();

			if (ImGui::Button("Push Relabel"))
			{
				pushRelabelVis.reset();
				currentVisualizer = &pushRelabelVis;
				currentAlgorithmText = "Push Relabel";
			}

			ImGui::Text(currentAlgorithmText.c_str());

			assert(currentVisualizer != nullptr);

			if (ImGui::Button("Run"))
			{
				currentVisualizer->runAlgorithm();
			}

			ImGui::SameLine();
			if (ImGui::Button("Step"))
			{
				currentVisualizer->step();
			}

			static float autoStepDelay = 1.f;

			ImGui::SameLine();
			if (ImGui::Button("Auto-Step"))
			{
				currentVisualizer->setAutoStepDelay(autoStepDelay);
				currentVisualizer->autoStep();
			}

			ImGui::SliderFloat("Auto-Step Delay", &autoStepDelay, 0.1f, 5.f);
			
			if (ImGui::Button("Show Min Cut"))
			{
				pushRelabelVis.saturatedCutVis();
			}
		}

		if (currentVisualizer != nullptr)
		{
			currentVisualizer->update(deltaTime.asSeconds());
		}

		if (ImGui::CollapsingHeader("Testing"))
		{
			static int numInstances = 10;
			ImGui::InputInt("test instances", &numInstances);
			static int numNodes = 30;
			ImGui::InputInt("test nodes", &numNodes);
			static int maxCapacity = 10;
			ImGui::InputInt("test capacity", &maxCapacity);
			static bool logToFile = false;
			ImGui::Checkbox("Log To File", &logToFile);

			if (ImGui::Button("Run Test"))
			{
				TestEnvironment te = TestEnvironment(numInstances, numNodes, maxCapacity, logToFile);
				te.runTest();
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