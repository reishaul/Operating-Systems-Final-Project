// reishaul1@gmail.com
/** 
 The Graph class represents an undirected graph using an adjacency list.
 It provides methods to add and remove edges, print the graph, and retrieve
 graph-related information such as the number of vertices and edges
*/
#include "Graph.hpp"
#include "algorithms/MST.hpp" // Include the MST algorithm for minimum spanning tree functionality
#include "algorithms/MaxFlow.hpp" // Include the MaxFlow algorithm
#include <stack>
#include <algorithm>


namespace graph {

/**
 * @brief Constructs a graph with the given number of vertices.
 * @param num_ver The number of vertices in the graph.
 * @throws std::invalid_argument if the number of vertices is invalid.
 */
Graph::Graph(int num_ver) : num_of_vertex(num_ver) {
    // if ( num_ver <= 0) {
    //     throw std::invalid_argument("Invalid number of vertex");
    // }//there is already a check in the server
    adj_list.resize(num_ver);
}

/**
 * @brief Adds an edge between two vertices with a specified weight.
 * @param src Source vertex.
 * @param dest Destination vertex.
 * @param w Weight of the edge.
 */
void Graph::addEdge(int src, int dest, int w) {
    validVertex(src);
    validVertex(dest);

    adj_list[src].push_back({dest, w});
    if (src != dest) {
        adj_list[dest].push_back({src, w});
    }
    //edges.push_back({src, dest, w});
}

/**
 * @brief Returns the number of vertices in the graph.
 * @return The number of vertices.
 */
int Graph::get_num_of_vertex() const{
    return num_of_vertex;
}

/**
 * @brief Retrieves all edges in the graph.
 * @param numEdges Reference to store the number of edges found.
 * @return A dynamically allocated array of edges.
 */
std::vector<std::tuple<int,int,int>> Graph::get_edges() const {
    std::vector<std::tuple<int,int,int>> edges;

    for (int i = 0; i < num_of_vertex; ++i) {
        for(const auto& edge : adj_list[i]) {
            if(i < edge.dest) { // to avoid duplicates in undirected graphs
                edges.emplace_back(i, edge.dest, edge.weight);
            }
        }
    }
    return edges;
}

/*
 * @brief Validates the vertex index check if the vertex index is within the valid range.
 * @param v Vertex index
 * @throws std::out_of_range if the vertex index is invalid.
 */
void Graph::validVertex(int v) const {
    if (v < 0 || v >= num_of_vertex){
        throw std::out_of_range("Vertex index out of range");
    }
}


//for the algserver 

//function to find the minimum spanning tree weight
long long Graph::mst_weight() const {
    return mst_weight_kruskal(*this);
}

/*
 * @brief Computes the maximum flow in the graph from source to sink
 * @param a Source vertex
 * @param b Sink vertex
 * @return The maximum flow value.
 */
int Graph::max_flow(int a, int b) const {
    int n = num_of_vertex;
    MaxFlow mf(n);

    // Initialize the MaxFlow object with the graph's edges
    for (int u = 0; u < n; ++u) {
        for (auto &e : adj_list[u]) {
            mf.addEdge(u, e.dest, e.weight);
        }
    }
    return mf.getMaxFlow(a, b);
}

/**
 * @brief Returns the neighbors of a vertex.
 * @param v Vertex index
 * @return A reference to the adjacency list of the vertex.
 * @throws std::out_of_range if the vertex index is invalid.
 */
const std::vector<Graph::Edge>& Graph::neighbors(int v) const {
    validVertex(v);
    return adj_list[v];
}
  
} 