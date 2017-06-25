#include "graph.h"
#include <iostream>

namespace slarx
{
	DirectedGraph Transpose(DirectedGraph& G)
	{
		DirectedGraph GT(G.size());
		for(int u = 0; u < G.size(); ++u)
		{
			for(int v : G[ u ])
			{
				GT[ v ].insert(u);
			}
		}

		return GT;
	}

	/// Implements finding SCC using Kosaraju's algorithm
	std::vector<std::set<int> > FindSCC(DirectedGraph& G)
	{
		std::vector<int> finish_time = DFS(G);
		std::vector<int> order(G.size());
		for(int i = 0; i < G.size(); ++i)
		{
			for(int j = 0; j < G.size(); ++j)
			{
				if(finish_time[ j ] == i)
				{
					order.push_back(j);
					break;
				}
			}
		}
		return DFS2(Transpose(G), order);
	}

	std::vector<int> DFS(const DirectedGraph& G)
	{
		std::vector<int> finish_time(G.size());
		std::vector<bool> visited(G.size(), false);
		int time = 0;
		for(int i = 0; i < G.size(); ++i)
		{
			if(!visited[ i ])
			{
				visited[ i ] = true;
				DFS_Visit(i, G, visited, finish_time, time);
			}
		}

		return finish_time;
	}

	std::vector<std::set<int> > DFS2(const DirectedGraph& G, const std::vector<int>& order)
	{
		std::vector<std::set<int> > SCC; // The SCCs
		std::vector<int> finish_time(G.size());
		std::vector<bool> visited(G.size(), false);
		for(int i : order)
		{
			std::set<int> component;
			if(!visited[ i ])
			{
				visited[ i ] = true;
				component.insert(i);
				DFS2_Visit(i, component, G, visited);
			}
			if(component.size() != 0)
				SCC.push_back(component);
		}
		
		return SCC;
	}

	void DFS2_Visit(int u, std::set<int>& component, const DirectedGraph& G, std::vector<bool>& visited)
	{
		for(int v : G[ u ])
		{
			if(!visited[ v ])
			{
				visited[ v ] = true;
				component.insert(v);
				DFS2_Visit(v, component, G, visited);
			}
		}
	}

	void DFS_Visit(int u, const DirectedGraph& G, std::vector<bool>& visited, std::vector<int>& finish_time, int& time)
	{
		for(int v : G[ u ])
		{
			if(!visited[ v ])
			{
				visited[ v ] = true;
				DFS_Visit(v, G, visited, finish_time, time);
			}
		}

		finish_time[u] = time;
		time++;
	}
}
