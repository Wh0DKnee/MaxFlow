#pragma once
#include <memory>
#include <fstream>

class Graph;

class Logger 
{
public:
	virtual void log(const std::string& s) = 0;

	virtual void init() {}
	virtual void close() {}
};

class ConsoleLogger : public Logger
{
public:
	virtual void log(const std::string& s) override;
};

class FileLogger : public Logger
{
public:
	virtual void log(const std::string& s) override;

	virtual void init() override;
	virtual void close() override;

private:
	std::ofstream outFile;
};

class ConsoleAndFileLogger : public Logger
{
public:

	// Inherited via Logger
	virtual void log(const std::string& s) override;

	virtual void init() override;
	virtual void close() override;
	
private:
	FileLogger fileLogger;
	ConsoleLogger consoleLogger;
};

class TestEnvironment
{
public:
	TestEnvironment(int numInstances, int numNodes, int maxCapacity, bool logToFile);

	void runTest() const;

private:
	void testConservationOfFlow(const Graph& graph) const;

	int numInstances;
	int numNodes;
	int maxCapacity;
	std::unique_ptr<Logger> logger;
};

