#pragma once
#ifndef SLARX_GRAPH_H_INCLUDED
#define SLARX_GRAPH_H_INCLUDED

#include <vector>
#include <set>
#include <utility>
#include <tuple>

// This file contains some useful graph algorithms used in the program

namespace slarx
{
	// Typedef for a simple graph representation using an adjacency list
	typedef std::vector<std::set<int> > DirectedGraph;
	// Computes transpose of graph G
	DirectedGraph Transpose(const DirectedGraph& G);
	/// Implements finding SCC using Kosaraju's algorithm
	std::vector<std::set<int> > FindSCC(const DirectedGraph& G);

	// Performs DFS on graph. Returns finish times on vertices
	std::vector<int> DFS(const DirectedGraph& G);
	// Auxillary function for DFS (this is the recursive part)
	void DFS_Visit(int u, const DirectedGraph& G, std::vector<bool>& visited, std::vector<int>& finish_time, int& time);

	// Performs DFS on G, but considers order specified by input vector
	std::vector<std::set<int> > DFS2(const DirectedGraph& G, const std::vector<int>& order);
	void DFS2_Visit(int u, std::set<int>& component, const DirectedGraph& G, std::vector<bool>& visited);
	// Finds set of vertices, reachable from a start vertex
	std::set<int> Reachable(const DirectedGraph& G, int start);
	// Auxilary function for the IsLanguageInfinite() Automaton function. Returns true if language is infinite and false otherwise
	bool IsLanguageInfinite_AUX(const DirectedGraph& G, State automaton_start_state, const std::set<State>& automaton_accepting_states);
	bool IsLanguageEmpty_AUX(const DirectedGraph& G, State automaton_start_state, const std::set<State>& automaton_accepting_states);
}

#endif // SLARX_GRAPH_H_INCLUDED
