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
	void FindSCC(DirectedGraph& G);
	// Performs DFS on graph. Returns finish times on vertices
	std::vector<int> DFS(DirectedGraph& G);
	// Auxillary function for DFS (this is the recursive part)
	void DFS_Visit(int u, DirectedGraph& G, std::vector<bool>& visited, std::vector<int>& finish_time, int& time);
}

#endif // SLARX_GRAPH_H_INCLUDED
