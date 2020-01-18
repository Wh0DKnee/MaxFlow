#include "Vertex.h"
#include <cassert>

void Edge::addResidualFlow(int amount)
{
	assert(flow + amount <= capacity);
	assert(backward->getFlow() >= amount);

	flow += amount;
	backward->addFlow(-amount);
}
