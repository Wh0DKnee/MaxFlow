#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include "Vertex.h"

// Not sure if this will need state at some point, so I made it a class instead of a namespace.
class DotWriter
{
public:
	static void Write(const std::vector<Vertex>& graph, const char* labelTags);
};

