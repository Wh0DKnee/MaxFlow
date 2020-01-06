#include "DotWriter.h"
#include <string>
#include <iomanip>

void DotWriter::Write(const std::vector<Vertex>& graph)
{
	static const char* prefix = R"(digraph {
    graph [pad="0.212,0.055" bgcolor="#d1d1d1"]
    node [style=filled])";

	static std::string generalVertexTagsPrePos = " [fillcolor = \"#000000\" pos = \"";
	static std::string generalVertexTagsPostPos = "!\" shape=circle label = \"\"]";

	std::ofstream outfile("graphviz/graph.dot");
	outfile << std::fixed;
	outfile << std::setprecision(2);
	outfile << prefix << std::endl;

	size_t index = 0;
	for (const auto& vert : graph)
	{
		outfile << index;
		outfile << generalVertexTagsPrePos;
		outfile << vert.posX;
		outfile << ",";
		outfile << vert.posY;
		outfile << generalVertexTagsPostPos;
		outfile << std::endl;
		++index;
	}

	outfile << "}" << std::endl;
	outfile.close();
}
