#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <tuple>
#include <stdexcept>
#include <iostream>

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

    //~Graph();// destructor
   
    //declaration of all the function we used in Graph.cpp
    void addEdge(int src, int dest, int weight = 1);

    //void addTreeEdge(int parent, int child, int w);

    void removeEdge(int src, int dest);

    void print_graph();

    int get_num_of_vertex() const;

    std::vector<std::tuple<int,int,int>> get_edges() const;

    bool has_eulerian_cycle() const;
    std::vector<int> get_eulerian_cycle() const;

    //bool isDirected() const { return directed; }

private:
    void validVertex(int v) const;
    bool removeNeighborEdge(int src, int dest);
    bool isConnected() const;
    int degree(int v) const;
};

} // namespace graph

#endif

