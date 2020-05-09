#include "Vertex.h"
#include <cassert>

void Edge::addResidualFlow(int amount)
{
	assert(backward != nullptr);
	assert(combined != nullptr);
	assert(flow + amount <= capacity);
	assert(backward->getFlow() >= amount);

	flow += amount;
	combined->flow += amount;

	backward->addFlow(-amount); // TODO: document this.
}

void Edge::highlight()
{
	assert(combined != nullptr);
	renderInfo.isHighlighted = true;
	combined->renderInfo.isHighlighted = true;
}

void Edge::resetHighlight()
{
	assert(combined != nullptr);
	renderInfo.reset();
	combined->renderInfo.reset();
}

void Edge::addFlow(int amount)
{
	assert(combined != nullptr);

	flow += amount;
	combined->flow += amount;
}
