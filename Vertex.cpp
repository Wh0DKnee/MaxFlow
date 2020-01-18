#include "Vertex.h"
#include <cassert>

void Edge::addResidualFlow(int amount)
{
	assert(flow + amount <= capacity);
	
	flow += amount;

}
