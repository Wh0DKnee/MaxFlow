#include "Algorithm.h"
#include <stack>
#include <limits>

bool Algorithm::DFS(Graph& graph, std::deque<Edge*>& outPath)
{
	std::deque<bool> visited(graph.size());
	std::stack<size_t> stack;
	std::vector<Edge*> visitedFrom(graph.size(), nullptr);
	stack.push(graph.getStart());

	while (!stack.empty())
	{
		size_t v = stack.top();
		stack.pop();
		if (!visited[v])
		{
			visited[v] = true;
			for (auto& edge : graph[v].edges)
			{
				if (edge.getRemainingCapacity() <= 0)
				{
					continue;
				}
				if (!visited[edge.targetNode])
				{
					// TODO: Understand why we can't just push edge onto the stack here
					// and omit the if(visisted[v]) check. 
					// See: https://stackoverflow.com/questions/25990706/breadth-first-search-the-timing-of-checking-visitation-status
					visitedFrom[edge.targetNode] = &edge;
				}
				if (edge.targetNode == graph.getTarget())
				{
					traceBack(visitedFrom, graph.getTarget(), outPath);
					graph.highlightPath(outPath);
					return true;
				}
				stack.push(edge.targetNode);
			}
		}
	}
	
	return false;
}

void Algorithm::traceBack(const std::vector<Edge*>& visitedFrom, size_t target, std::deque<Edge*>& outPath)
{
	outPath.clear();
	size_t current = target;
	while (visitedFrom[current] != nullptr)
	{
		outPath.push_front(visitedFrom[current]);
		current = visitedFrom[current]->startNode;
	}
	return;
}

void Algorithm::fordFulkerson(Graph& graph)
{
	std::deque<Edge*> path;
	while (DFS(graph, path))
	{
		int minResidualCap = std::numeric_limits<int>::max();
		for (const auto& edge : path)
		{
			if (edge->getRemainingCapacity() < minResidualCap)
			{
				minResidualCap = edge->getRemainingCapacity();
			}
		}
		for (auto& edge : path)
		{
			edge->addResidualFlow(minResidualCap);
		}
	}
}

void Algorithm::fordFulkersonStep(Graph& graph)
{
	std::deque<Edge*> path;
	if(DFS(graph, path))
	{
		int minResidualCap = std::numeric_limits<int>::max();
		for (const auto& edge : path)
		{
			if (edge->getRemainingCapacity() < minResidualCap)
			{
				minResidualCap = edge->getRemainingCapacity();
			}
		}
		for (auto& edge : path)
		{
			edge->addResidualFlow(minResidualCap);
		}
	}
}
