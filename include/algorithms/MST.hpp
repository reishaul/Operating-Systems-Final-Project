#pragma once
#include <vector>
#include <tuple>

// Forward declaration
namespace graph {
    class Graph;
}

long long mst_weight_kruskal(const graph::Graph& G);
