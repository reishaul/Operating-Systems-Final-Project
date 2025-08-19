#pragma once
#include <vector>
#include <stdexcept>

/**
 * Directed graph represented as a Digraph object
 */
class Digraph {
    int n;
    std::vector<std::vector<int>> g;// adjacency list
public:
    explicit Digraph(int n_) : n(n_), g(n_) {
        if (n_<=0) throw std::invalid_argument("bad n");
    }

    void addEdge(int u,int v){ if(u<0||v<0||u>=n||v>=n) throw std::out_of_range("arc"); g[u].push_back(v); }//add edge
    int size() const { return n; }
    const std::vector<int>& adj(int u) const { return g[u]; }
};
