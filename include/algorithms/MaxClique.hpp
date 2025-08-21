#pragma once
#include "Graph.hpp"
#include <vector>

namespace graph {

// Finds the maximum clique in a graph.
std::vector<int> find_max_clique(const Graph& G);

}
