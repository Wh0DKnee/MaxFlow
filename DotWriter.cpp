#include "DotWriter.h"
#include <string>
#include <iomanip>

void DotWriter::Write(const std::vector<Vertex>& graph, const char* labelTags)
{
	//At present, most device-independent units are either inches or points, which we take as 72 points per inch.
	//dpi: This specifies the expected number of pixels per inch on a display device
	// 72 points per inch
	// 96 pixels per inch
	// 720 Breite -> 10 inch x 3 inch -> 960 x 289 pixel
	// 1600 x 900 -> 16.66666 inch x 9.375 -> 1200 x 675
	static const char* prefix = R"(digraph {
	splines=ortho
	overlap=true
	sep=true
	dpi=96
	viewport="1200,675"
    graph [pad="0.212,0.055" bgcolor="#d1d1d1" size="12,8!"]
    node [fillcolor = "#000000" style=filled shape=circle width=0.1 fixedsize=true label = ""]
	edge [arrowhead=halfopen arrowsize=1 fontsize=15 penwidth=1])";

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
			outfile << index << neighbor.index;
			outfile << "[style=solid label=\"5/10\"";
			outfile << "pos=\"";
			Point delta = graph[neighbor.index].pos - vert.pos;
			Point middle = vert.pos + (delta / 2.f);
			Point perpendicular = Point(delta.y, -delta.x);
			Point pos = middle + perpendicular.normalize() * 20.f;
			outfile << pos.x << "," << pos.y;
			outfile << "!\" " << labelTags << "]";
			outfile << std::endl;
			outfile << index << " -> " << index << neighbor.index << " -> " << neighbor.index;
			outfile << std::endl;
		}
		++index;
	}

	outfile << "}" << std::endl;

	outfile.close();
}
