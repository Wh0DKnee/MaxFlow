#include "Algorithm.h"
#include <algorithm>
#include <stack>
#include <queue>
#include <limits>
#include <cassert>

GraphSearchResult Algorithm::DFS(Graph& graph, std::deque<Edge*>& outPath)
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
					return GraphSearchResult(true, visited);
				}
				stack.push(edge.targetNode);
			}
		}
	}
	
	return GraphSearchResult(false, visited);
}

GraphSearchResult Algorithm::BFS(Graph& graph, std::deque<Edge*>& outPath)
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
			return GraphSearchResult(true, visited);
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

	return GraphSearchResult(false, visited);
}

bool Algorithm::dinicDFS(Graph& graph, std::deque<Edge*>& outPath)
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
			int validEdgeCount = 0;
			for (auto& edge : graph[v].edges)
			{
				if (edge.getRemainingCapacity() <= 0 || !edge.isInLevelGraph(graph))
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
				++validEdgeCount;
			}

			if (validEdgeCount == 0)
			{
				graph.setLevel(v, -1); // Remove vertex from graph if we backtrack from it.
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
	while (BFS(graph, path).pathFound)
	{
		exhaustPath(path);
	}
}

void Algorithm::edmondsKarpStep(Graph& graph)
{
	std::deque<Edge*> path;
	if (BFS(graph, path).pathFound)
	{
		exhaustPath(path);
	}
}

void Algorithm::fordFulkerson(Graph& graph)
{
	std::deque<Edge*> path;
	while (DFS(graph, path).pathFound)
	{
		exhaustPath(path);
	}
}

void Algorithm::fordFulkersonStep(Graph& graph)
{
	std::deque<Edge*> path;
	if(DFS(graph, path).pathFound)
	{
		exhaustPath(path);
	}
}

void Algorithm::dinic(Graph& graph)
{
	std::deque<Edge*> path;
	while (BFS(graph, path).pathFound)
	{
		dinicBlockingFlow(graph);
		graph.resetDinicLevels();
	}
}

void Algorithm::dinicStep(Graph& graph)
{
	std::deque<Edge*> path;
	if (BFS(graph, path).pathFound)
	{
		dinicBlockingFlow(graph);
	}
}

void Algorithm::dinicBlockingFlow(Graph& graph)
{
	std::deque<Edge*> path;
	while (dinicDFS(graph, path))
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

void Algorithm::pushRelabel(Graph& graph)
{
	pushRelabelInit(graph);
	while (!graph.verticesWithExcess.empty())
	{
		pushRelabelStep(graph); // will check the empty condition again, but it's constant time.
	}
}

void Algorithm::pushRelabelStep(Graph& graph)
{
	if (!graph.verticesWithExcess.empty())
	{
		auto front = graph.verticesWithExcess.front();
		bool foundValidEdge = false;

		for (auto& edge : graph[front].edges)
		{
			if ((edge.getRemainingCapacity() > 0)
				&& (graph[edge.startNode].getHeight() == (graph[edge.targetNode].getHeight() + 1)))
			{
				foundValidEdge = true;
				push(graph, edge, graph[front].getExcess());
				if (graph[front].getExcess() == 0)	// If all excess got pushed...
				{
					graph.verticesWithExcess.pop();	// ...pop it off the queue...
					break;							// and stop trying to push excess.
				}
			}
		}

		if (!foundValidEdge)
		{
			graph[front].incrementHeight(); // relabel
		}
	}
}

void Algorithm::push(Graph& graph, Edge& edge, int excess)
{
	int amountToPush = std::min(excess, edge.getRemainingCapacity());
	edge.addResidualFlow(amountToPush);
	if (graph[edge.targetNode].getExcess() == 0 
		&& edge.targetNode != graph.getStart()		// don't wanna push source to active nodes
		&& edge.targetNode != graph.getTarget())	// also not the target
	{
		graph.verticesWithExcess.push(edge.targetNode);
	}
	graph[edge.startNode].addExcess(-amountToPush);
	graph[edge.targetNode].addExcess(amountToPush);
}

void Algorithm::pushRelabelInit(Graph& graph)
{
	for (auto& e : graph[graph.getStart()].edges)
	{
		int amountToPush = e.getRemainingCapacity();
		if (amountToPush == 0) 
		{ 
			continue; 
		}

		e.addResidualFlow(amountToPush);
		if (graph[e.targetNode].getExcess() == 0)		// first time we add excess? 
		{
			graph.verticesWithExcess.push(e.targetNode);// then push it onto queue.
		}
		graph[e.targetNode].addExcess(amountToPush);
	}
}
