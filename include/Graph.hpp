#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <tuple>
#include <stdexcept>
#include <iostream>
#include <functional>

// Forward declarations
namespace graph {
    class Graph;
}
long long mst_weight_kruskal(const graph::Graph& G);

namespace graph {

class Graph {
public:

    struct Edge {
        int dest;
        int weight; // for unweighted graphs, keep as 1
    };
private:
    int num_of_vertex;
    std::vector<std::vector<Edge>> adj_list; // adjacency list representation

public:

    explicit Graph(int num_ver);//constructor
   
    //declaration of all the function we used in Graph.cpp
    void addEdge(int src, int dest, int weight = 1);

    void removeEdge(int src, int dest);

    void print_graph();

    int get_num_of_vertex() const;

    std::vector<std::tuple<int,int,int>> get_edges() const;

    bool has_eulerian_cycle() const;
    std::vector<int> get_eulerian_cycle() const;

    //for algserver:

    long long mst_weight() const;//implementation in cpp file

    auto hamiltonian_cycle() const {
        // Implement Hamiltonian cycle finding logic
        // This is a placeholder, actual implementation needed
        return std::vector<int>{};
    }

    int max_flow(int a, int b) const;

    std::vector<std::vector<int>> strongly_connected_components() const;



private:
    void validVertex(int v) const;
    bool removeNeighborEdge(int src, int dest);
    bool isConnected() const;
    int degree(int v) const;
};

} // namespace graph

#endif

