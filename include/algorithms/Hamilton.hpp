#pragma once
#include "Graph.hpp"
#include <vector>

// Finds a Hamiltonian cycle in the given graph, if it exists.
std::vector<int> find_hamiltonian_cycle(const graph::Graph& G);
