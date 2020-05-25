#include "TestEnvironment.h"
#include <chrono>
#include <iostream>
#include <cassert>
#include "Algorithm.h"

void TestEnvironment::runTest() const
{
	std::cout << "Testing with " + std::to_string(numInstances);
	std::cout << " instances with " + std::to_string(numNodes);
	std::cout << " nodes each and maximum capacity of " + std::to_string(maxCapacity) + "." << std::endl;
	std::cout << std::endl;

	for (int i = 0; i < numInstances; ++i)
	{
		unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
		Graph ffGraph = Graph(numNodes, maxCapacity, 1000, 1000, 0.f, seed);
		Graph ekGraph = Graph(numNodes, maxCapacity, 1000, 1000, 0.f, seed);
		Graph dinicGraph = Graph(numNodes, maxCapacity, 1000, 1000, 0.f, seed);
		Graph prGraph = Graph(numNodes, maxCapacity, 1000, 1000, 0.f, seed);

		Algorithm::fordFulkerson(ffGraph);
		Algorithm::edmondsKarp(ekGraph);
		Algorithm::dinic(dinicGraph);
		Algorithm::pushRelabel(prGraph);

		// Make sure all algorithms calculate the same max flow
		assert(ffGraph.getFlow() == ekGraph.getFlow());
		assert(ekGraph.getFlow() == dinicGraph.getFlow());
		assert(dinicGraph.getFlow() == prGraph.getFlow());

		testConservationOfFlow(ffGraph);
		testConservationOfFlow(ekGraph);
		testConservationOfFlow(dinicGraph);
		testConservationOfFlow(prGraph);
	}

}

void TestEnvironment::testConservationOfFlow(const Graph& graph) const
{
	size_t index = 0;
	for (const auto& vert : graph)
	{
		if (index == graph.getSource() || index == graph.getSink())
		{
			continue; // no flow conservation for source and sink.
		}

		assert(vert.getIncomingFlow() == vert.getOutGoingFlow());

		++index;
	}
}
