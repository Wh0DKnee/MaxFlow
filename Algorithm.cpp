#include "Algorithm.h"
#include <stack>
#include <limits>

std::deque<size_t> Algorithm::DFS(const Graph& graph)
{
	std::deque<bool> visited(graph.size());
	std::stack<size_t> stack;
	std::vector<size_t> visitedFrom(graph.size(), std::numeric_limits<size_t>::max());
	stack.push(graph.getStart());

	while (!stack.empty())
	{
		size_t v = stack.top();
		stack.pop();
		if (!visited[v])
		{
			visited[v] = true;
			for (const auto& edge : graph[v].edges)
			{
				if (edge.getRemainingCapacity() <= 0)
				{
					continue;
				}
				if (!visited[edge.targetNode])
				{
					visitedFrom[edge.targetNode] = v;
				}
				if (edge.targetNode == graph.getTarget())
				{
					return traceBack(visitedFrom, graph.getTarget());
				}
				stack.push(edge.targetNode);
			}
		}
	}
	
	return std::deque<size_t>();
}

std::deque<size_t> Algorithm::traceBack(const std::vector<size_t>& visitedFrom, size_t target)
{
	std::deque<size_t> path;
	size_t current = target;
	while (current != std::numeric_limits<size_t>::max())
	{
		path.push_front(current);
		current = visitedFrom[current];
	}
	return path;
}
