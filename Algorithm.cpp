#include "Algorithm.h"
#include <stack>
#include <queue>
#include <limits>
#include <cassert>

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
					return true;
				}
				stack.push(edge.targetNode);
			}
		}
	}
	
	return false;
}

bool Algorithm::BFS(Graph& graph, std::deque<Edge*>& outPath)
{
	std::deque<bool> visited(graph.size());
	std::queue<size_t> queue;
	std::vector<Edge*> visitedFrom(graph.size(), nullptr);
	queue.push(graph.getStart());
	visited[graph.getStart()] = true;
	graph.setLevel(graph.getStart(), 0); // for Dinic level graph

	while (!queue.empty())
	{
		size_t v = queue.front();
		queue.pop();

		if (v == graph.getTarget())
		{
			traceBack(visitedFrom, graph.getTarget(), outPath);
			return true;
		}

		for (auto& edge : graph[v].edges)
		{
			if (edge.getRemainingCapacity() <= 0)
			{
				continue;
			}

			if (!visited[edge.targetNode])
			{
				visited[edge.targetNode] = true;
				visitedFrom[edge.targetNode] = &edge;
				graph.setLevel(edge.targetNode, graph.getLevel(edge.startNode) + 1);
				queue.push(edge.targetNode);
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

void Algorithm::edmondsKarp(Graph& graph)
{
	std::deque<Edge*> path;
	while (BFS(graph, path))
	{
		exhaustPath(path);
	}
}

void Algorithm::edmondsKarpStep(Graph& graph)
{
	std::deque<Edge*> path;
	if (BFS(graph, path))
	{
		exhaustPath(path);
	}
}

void Algorithm::fordFulkerson(Graph& graph)
{
	std::deque<Edge*> path;
	while (DFS(graph, path))
	{
		exhaustPath(path);
	}
}

void Algorithm::fordFulkersonStep(Graph& graph)
{
	std::deque<Edge*> path;
	if(DFS(graph, path))
	{
		exhaustPath(path);
	}
}

void Algorithm::exhaustPath(std::deque<Edge*>& path)
{
	int minResidualCap = getMinResidualCapacity(path);
	for (auto& edge : path)
	{
		edge->addResidualFlow(minResidualCap);
	}
}

int Algorithm::getMinResidualCapacity(const std::deque<Edge*>& path, Edge** outLimitingEdge /*= nullptr*/)
{
	int minResidualCap = std::numeric_limits<int>::max();
	for (const auto& edge : path)
	{
		if (edge->getRemainingCapacity() < minResidualCap)
		{
			minResidualCap = edge->getRemainingCapacity();
			if (outLimitingEdge != nullptr)
			{
				*outLimitingEdge = edge;
			}
		}
	}
	return minResidualCap;
}
