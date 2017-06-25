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
	DirectedGraph Transpose(DirectedGraph& G);
	/// Implements finding SCC using Kosaraju's algorithm
	std::vector<std::set<int> > FindSCC(DirectedGraph& G);

	// Performs DFS on graph. Returns finish times on vertices
	std::vector<int> DFS(const DirectedGraph& G);
	// Auxillary function for DFS (this is the recursive part)
	void DFS_Visit(int u, const DirectedGraph& G, std::vector<bool>& visited, std::vector<int>& finish_time, int& time);

	// Performs DFS on G, but considers order specified by input vector
	std::vector<std::set<int> > DFS2(const DirectedGraph& G, const std::vector<int>& order);
	void DFS2_Visit(int u, std::set<int>& component, const DirectedGraph& G, std::vector<bool>& visited);
}

#endif // SLARX_GRAPH_H_INCLUDED
