#include "Vertex.h"
#include "Graph.h"
#include <cassert>


void Edge::addResidualFlow(int amount)
{
	assert(backward != nullptr);
	assert(combined != nullptr);
	assert(amount <= residualCap);

	addFlow(amount);
	backward->addFlow(-amount); 
}

void Edge::highlight()
{
	assert(combined != nullptr);
	renderInfo.highlight();
	combined->renderInfo.highlight();
}

void Edge::resetHighlight()
{
	assert(combined != nullptr);
	renderInfo.resetHighlight();
	combined->renderInfo.resetHighlight();
}

void Edge::setColor(sf::Color c)
{
	assert(combined != nullptr);
	renderInfo.setColor(c);
	combined->renderInfo.setColor(c);
}

bool Edge::isInLevelGraph(const Graph& graph) const
{
	return (getResidualCapacity() > 0) 
		&& ((graph.getLevel(startNode) + 1) == graph.getLevel(targetNode));
}

void Edge::addFlow(int amount)
{
	assert(combined != nullptr);

	if (isOriginal)
	{
		combined->renderedFlow += amount;
	}

	residualCap -= amount;
	combined->residualCap -= amount;
}
