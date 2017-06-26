#include "slarx.h"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <queue>

namespace slarx
{
	DirectedGraph Transpose(const DirectedGraph& G)
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

	// Implements finding SCC using Kosaraju's algorithm
	std::vector<std::set<int> > FindSCC(const DirectedGraph& G)
	{
		std::vector<int> finish_time = DFS(G);
		std::vector<int> order;
		for(int i = G.size()-1; i >= 0; --i)
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

		finish_time[ u ] = time;
		time++;
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

	

	// Runs BFS. Returns set of vertices, reachable from start
	std::set<int> Reachable(const DirectedGraph& G, int start)
	{
		std::set<int> reachable;
		reachable.insert(start);
		std::queue<int> Q;
		Q.push(start);
		while(!Q.empty())
		{
			int u = Q.front();
			Q.pop();
			for(int v : G[ u ])
			{
				if(reachable.find(v) == reachable.end())
				{
					reachable.insert(v);
					Q.push(v);
				}
			}
		}
		
		return reachable;
	}

	bool IsLanguageInfinite_AUX(const DirectedGraph& G, State automaton_start_state, const std::set<State>& automaton_accepting_states)
	{
		std::vector<std::set<int> > SCC = FindSCC(G);
		auto new_end = std::remove_if(SCC.begin(), SCC.end(), [&G](const std::set<int>& s){ auto bg = s.begin(); return (s.size() <= 1) && (G[*bg].find(*bg) == G[*bg].end()); });
		SCC.erase(new_end, SCC.end());
		int start_state = automaton_start_state.GetValue();
		std::set<int> final_states;
		for(State s : automaton_accepting_states)
		{
			final_states.insert(s.GetValue());
		}
		std::set<int> reachable_from_start_state = Reachable(G, start_state);
		for(std::set<int> scc : SCC)
		{
			// Compute all vertices reachable from the current strongly connected component. Obviously if a vertex is reachable from one scc vertex, it is reachable from all of them
			std::set<int> reachable_from_scc = Reachable(G, *scc.begin());
			// The scc must be reachable from the start state if the language is infinite
			if(reachable_from_start_state.find(*scc.begin()) != reachable_from_start_state.end())
			{
				for(int fs : final_states)
				{
					// If any final vertex is reachable from the scc, then we can go through it an arbitrary amount of times and proceed to that final vertex
					if(scc.find(fs) != scc.end())
					{
						return true;
					}
				}
			}
		}

		return false;
	}

	bool IsLanguageEmpty_AUX(const DirectedGraph& G, State automaton_start_state, const std::set<State>& automaton_accepting_states)
	{
		int start_state = automaton_start_state.GetValue();
		std::set<int> final_states;
		for(State s : automaton_accepting_states)
		{
			final_states.insert(s.GetValue());
		}
		std::set<int> reachable_from_start_state = Reachable(G, start_state);
		for(int fs : final_states)
		{
			if(reachable_from_start_state.find(fs) != reachable_from_start_state.end())
			{
				return false;
			}
		}

		return true;
	}
}
