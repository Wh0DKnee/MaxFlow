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
					visitedFrom[edge.targetNode] = &edge;
				}
				if (edge.targetNode == graph.getTarget())
				{
					traceBack(visitedFrom, graph.getTarget(), outPath);
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

	}
}
