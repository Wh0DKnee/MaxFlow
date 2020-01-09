#include "DotWriter.h"
#include <string>
#include <iomanip>

void DotWriter::Write(const std::vector<Vertex>& graph, const char* labelTags)
{
	static const char* prefix = R"(digraph {
	splines=compound
    graph [pad="0.212,0.055" bgcolor="#d1d1d1" dpi=100 size="12,8!"]
    node [fillcolor = "#000000" style=filled shape=circle width=0.2 fixedsize=true label = ""])";

	static std::string generalVertexTagsPrePos = " [pos = \"";
	static std::string generalVertexTagsPostPos = "!\"]";

	std::ofstream outfile("graphviz/graph.dot");
	outfile << std::fixed;
	outfile << std::setprecision(2);
	outfile << prefix << std::endl;

	size_t index = 0;
	for (const auto& vert : graph)
	{
		outfile << index;
		outfile << generalVertexTagsPrePos;
		outfile << vert.pos.x;
		outfile << ",";
		outfile << vert.pos.y;
		outfile << generalVertexTagsPostPos;
		outfile << std::endl;
		++index;
	}

	index = 0;
	for (const auto& vert : graph)
	{
		for (const auto& neighbor : vert.neighbors)
		{
			outfile << index;
			outfile << " -> ";
			outfile << neighbor.index;
			outfile << labelTags;
			outfile << std::endl;
		}
		++index;
	}

	outfile << "}" << std::endl;

	outfile.close();
}
