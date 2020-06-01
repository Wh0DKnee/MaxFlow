#include "TestEnvironment.h"
#include <chrono>
#include <ctime> 
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <cassert>
#include <algorithm>
#include "Algorithm.h"

#ifdef _WIN32
#include <windows.h>
#define SYSERROR()  GetLastError()
#else
#include <errno.h>
#define SYSERROR()  errno
#endif

void ConsoleLogger::log(const std::string& s)
{
	std::cout << s;
}

void FileLogger::log(const std::string& s)
{
	outFile << s;
}

void FileLogger::init()
{
	auto time = std::chrono::system_clock::now();
	std::time_t result = std::chrono::system_clock::to_time_t(time);
	char s[26];
	ctime_s(s, sizeof s, &result);
	std::string str(s);
	str.pop_back(); // remove new line character
	str += ".txt";
	std::string::iterator end_pos = std::remove(str.begin(), str.end(), ' ');
	str.erase(end_pos, str.end()); // remove spaces
	end_pos = std::remove(str.begin(), str.end(), ':');
	str.erase(end_pos, str.end()); // remove colons

	outFile.open("logs\\" + str, std::fstream::out | std::fstream::trunc);

	if (!outFile.is_open())
	{
		std::cerr << "Failed to open file : " << SYSERROR() << std::endl;
	}
}

void FileLogger::close()
{
	outFile.close();
}

void ConsoleAndFileLogger::log(const std::string& s)
{
	consoleLogger.log(s);
	fileLogger.log(s);
}

void ConsoleAndFileLogger::init()
{
	consoleLogger.init();
	fileLogger.init();
}

void ConsoleAndFileLogger::close()
{
	consoleLogger.close();
	fileLogger.close();
}


TestEnvironment::TestEnvironment(int numInstances, int numNodes, int maxCapacity, bool logToFile)
	:numInstances(numInstances), numNodes(numNodes), maxCapacity(maxCapacity)
{
	if (logToFile)
	{
		logger = std::make_unique<ConsoleAndFileLogger>();
	}
	else
	{
		logger = std::make_unique<ConsoleLogger>();
	}
}

void TestEnvironment::runTest() const
{
	logger->init();
	logger->log("Testing " + std::to_string(numInstances));
	logger->log(" instances with " + std::to_string(numNodes));
	logger->log(" nodes each and maximum capacity of " + std::to_string(maxCapacity) + ".\n\n");

	for (int i = 0; i < numInstances; ++i)
	{
		logger->log("Testing progess: instance " + std::to_string(i + 1) + "/" + std::to_string(numInstances) + "\n");

		unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
		Graph ffGraph = Graph(numNodes, maxCapacity, 1000, 1000, 0.f, seed);
		Graph ekGraph = Graph(numNodes, maxCapacity, 1000, 1000, 0.f, seed);
		Graph dinicGraph = Graph(numNodes, maxCapacity, 1000, 1000, 0.f, seed);
		Graph prGraph = Graph(numNodes, maxCapacity, 1000, 1000, 0.f, seed);
		std::vector<Graph*> testGraphs = { &ffGraph, &ekGraph, &dinicGraph, &prGraph };

		Algorithm::fordFulkerson(ffGraph);
		Algorithm::edmondsKarp(ekGraph);
		Algorithm::dinic(dinicGraph);
		Algorithm::pushRelabel(prGraph);

		// Make sure all algorithms calculate the same max flow
		assert(ffGraph.getFlow() == ekGraph.getFlow());
		assert(ekGraph.getFlow() == dinicGraph.getFlow());
		assert(dinicGraph.getFlow() == prGraph.getFlow());

		int flow1 = ffGraph.getFlow();
		
		logger->log("Max flow equal to " + std::to_string(ffGraph.getFlow()) + " for all algorithms.\n");

		testConservationOfFlow(ffGraph);
		testConservationOfFlow(ekGraph);
		testConservationOfFlow(dinicGraph);
		testConservationOfFlow(prGraph);

		logger->log("Conservation of flow and capacity contraints fulfilled for all algorithms. \n\n");

		for (auto& g : testGraphs)
		{
			g->reset();
			for (auto& e : (*g)[g->getSource()].edges)
			{
				if (e.isOriginal)
				{
					e.addCapacity(-e.getResidualCapacity()); // set capacity to zero on first edge.
				}

			}
		}

		for (auto& g : testGraphs)
		{
			auto& firstEdge = (*g)[g->getSource()].edges[0];
			firstEdge.addCapacity(1);
		}

		Algorithm::fordFulkerson(ffGraph);
		Algorithm::edmondsKarp(ekGraph);
		Algorithm::dinic(dinicGraph);
		Algorithm::pushRelabel(prGraph);

		assert(ffGraph.getFlow() == ekGraph.getFlow());
		assert(ekGraph.getFlow() == dinicGraph.getFlow());
		assert(dinicGraph.getFlow() == prGraph.getFlow());

		int flow2 = ffGraph.getFlow();

		assert(flow2 <= 1);

		//logger->log("Changed capacity on outgoing edge to zero, resulting flow was: ");
		//logger->log(std::to_string(flow2));
		//logger->log(".\n\n");
	}

	logger->log("All tests passed. \n\n");
	logger->close();
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