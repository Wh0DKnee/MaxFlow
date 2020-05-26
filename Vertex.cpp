#include "Vertex.h"
#include "Graph.h"
#include <cassert>


void Edge::addCapacity(int c)
{
	residualCap += c;
	if (combined != nullptr)
	{
		combined->residualCap += c;
	}
}

int Edge::getFlow() const
{
	if (!isOriginal)
	{
		return 0; // backward edges don't really have a concept of flow.
	}

	assert(backward != nullptr);
	return backward->getResidualCapacity(); // flow on an edge is equal to residualCapacity on its backward edge.
}

void Edge::addResidualFlow(int amount)
{
	assert(backward != nullptr);
	assert(combined != nullptr);
	assert(amount <= residualCap); // because of this assert, capacity constraints can never be violated.

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

int Vertex::getOutGoingFlow() const
{
	int flow = 0;
	for (const auto& e : edges)
	{
		if (e.isOriginal)
		{
			flow += e.getFlow();
		}
	}
	return flow;
}

int Vertex::getIncomingFlow() const
{
	int flow = 0;
	for (const auto& e : edges)
	{
		if(!e.isOriginal)
		{
			// Residual capacity on outgoing backward edges is equal
			// to flow on incoming original edges.
			flow += e.getResidualCapacity();
		}
	}
	return flow;
}
