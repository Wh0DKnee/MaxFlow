#pragma once

class Graph;

class TestEnvironment
{
public:
	TestEnvironment(int numInstances, int numNodes, int maxCapacity)
		: numInstances(numInstances), numNodes(numNodes), maxCapacity(maxCapacity) {}

	void runTest() const;

private:
	void testConservationOfFlow(const Graph& graph) const;

	int numInstances;
	int numNodes;
	int maxCapacity;
};

