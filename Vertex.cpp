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
	renderInfo.highlight();
	combined->renderInfo.highlight();
}

void Edge::resetHighlight()
{
	assert(combined != nullptr);
	renderInfo.resetHighlight();
	combined->renderInfo.resetHighlight();
}

void Edge::addFlow(int amount)
{
	assert(combined != nullptr);

	flow += amount;
	combined->flow += amount;
}

void RenderInfo::highlight()
{
	isHighlighted = true;
	color = highlightColor;
}

void RenderInfo::resetHighlight()
{
	isHighlighted = false;
	color = regularColor;
}

void RenderInfo::setRegularColor(sf::Color c)
{
	regularColor = c;
	if (!isHighlighted)
	{
		color = c;
	}
}

void RenderInfo::setHighlightColor(sf::Color c)
{
	highlightColor = c;
	if (isHighlighted)
	{
		color = c;
	}
}
