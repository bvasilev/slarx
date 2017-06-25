#include "graph.h"

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
	void FindSCC(DirectedGraph& G)
	{
		
	}

	std::vector<int> DFS(DirectedGraph& G)
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
	}

	void DFS_Visit(int u, DirectedGraph& G, std::vector<bool>& visited, std::vector<int>& finish_time, int& time)
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
