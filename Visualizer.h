#pragma once

class Visualizer
{
public:
	virtual void update(class Graph& graph) = 0;

	virtual void reset() = 0;
};

