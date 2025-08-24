// reishaul1@gmail.com
/** 
 The Graph class represents an undirected graph using an adjacency list.
 It provides methods to add and remove edges, print the graph, and retrieve
 graph-related information such as the number of vertices and edges.
*/
#include "graph.hpp"
// #include "Algorithms/MST.hpp" // Include the MST algorithm for minimum spanning tree functionality  
// #include "Algorithms/MaxFlow.hpp" // Include the MaxFlow algorithm
#include <stack>
#include <algorithm>


namespace graph {

/**
 * @brief Constructs a graph with the given number of vertices.
 * @param num_ver The number of vertices in the graph.
 * @throws std::invalid_argument if the number of vertices is invalid.
 */
Graph::Graph(int num_ver) : num_of_vertex(num_ver) {
    if ( num_ver <= 0) {
        throw std::invalid_argument("Invalid number of vertex");
    }
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
 * @brief Removes an edge between two vertices.
 * @param src Source vertex
 * @param dest Destination vertex
 * @throws runtime_error if the edge does not exist.
 */
void Graph::removeEdge(int src, int dest) {
    validVertex(src);
    validVertex(dest);
    bool removed = removeNeighborEdge(src, dest);
    if(!removed) {
        throw std::runtime_error("Edge not found in the graph");
    }
    if ( src != dest) {
        removed = removeNeighborEdge(dest, src)||removed;
    }
}


/**
 * @brief Prints the adjacency list representation of the graph
 */
void Graph::print_graph() {//printing the graph in a logical way
    for (int i = 0; i < num_of_vertex; i++) {
        std::cout << "Node " << i << ": ";
        for(const auto& edge : adj_list[i]) {
            std::cout << "(" << edge.dest << ", weight: " << edge.weight << ") ";
        }
        std::cout << std::endl;
    }
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

void Graph::validVertex(int v) const {
    if (v < 0 || v >= num_of_vertex)
        throw std::out_of_range("Vertex index out of range");
}


/**
 * @brief Removes an edge from the adjacency list.
 * for undirected graphs, it removes the edge in both directions.
 * @param src Source vertex
 * @param dest Destination vertex
 * @return true if the edge was removed, false if it was not found.
 */
bool Graph::removeNeighborEdge(int src, int dest) {
    auto& edges = adj_list[src];
    for (auto it = edges.begin(); it != edges.end(); ++it) {
        if (it->dest == dest) {
            edges.erase(it);
            return true;
        }
    }
    return false;
}

/**
 * @brief Returns the degree of a vertex.
 * @param v Vertex index
 * @return The degree of the vertex.
 * @throws std::out_of_range if the vertex index is invalid.
 */
int Graph::degree(int v) const {
    validVertex(v);
    return static_cast<int>(adj_list[v].size());
}


/**
 * @brief Checks if the graph is connected ignoring isolated vertices.
 * @return true if the graph is connected, false otherwise.
 */
bool Graph::isConnected() const {
    int a=-1;
    for (int i = 0; i < num_of_vertex; ++i) {
        if (degree(i) > 0) {
            a = i;
            break;
        }
    }
    if (a == -1) return true; // If there is no any edges, the graph is trivially connected

    std::vector<bool> visited(num_of_vertex, false);
    std::stack<int> stack;

    stack.push(a);
    visited[a] = true;

    // Perform DFS to check connectivity
    // We use a stack to avoid recursion depth issues in large graphs
    while (!stack.empty()) {
        int v = stack.top();
        stack.pop();

        for (const auto& edge : adj_list[v]) {
            if (!visited[edge.dest]) {
                visited[edge.dest] = true;
                stack.push(edge.dest);
            }
        }
    }

    for(int i = 0; i < num_of_vertex; ++i) {
        if (degree(i) > 0 && !visited[i]) {
            return false; // Found a vertex with edges that is not visited
        }
    }
    return true; // All vertices with edges are connected
}

/**
 * @brief Checks if the graph has an Eulerian cycle.
 * An Eulerian cycle exists if the graph is connected and all vertices have even degree.
 * @return true if the graph has an Eulerian cycle, false otherwise.
 */
bool Graph::has_eulerian_cycle() const {
    if (!isConnected()) return false;

    for (int v = 0; v < num_of_vertex; ++v) {
        if (degree(v) % 2 != 0) {
            return false; // Vertex with odd degree found
        }
    }
    return true; // All vertices have even degree
}

/**
 * @brief Finds an Eulerian cycle in the graph if it exists.
 * Uses Hierholzer's algorithm to find the cycle.
 * @return A vector containing the vertices in the Eulerian cycle.
 * @throws std::runtime_error if no Eulerian cycle exists.
 */
std::vector<int> Graph::get_eulerian_cycle() const {

    if (!has_eulerian_cycle())// Check if the graph has an Eulerian cycle
        throw std::runtime_error("Eulerian circuit does not exist");

    int a = -1;
    long long totalHalfEdges = 0;// Count total half-edges to find a starting point
    // Find a starting vertex with edges
    for (int v = 0; v < num_of_vertex; ++v) {
        if (!adj_list[v].empty() && a == -1) a = v;
        totalHalfEdges += adj_list[v].size();
    }
    if (a == -1) return {0}; // single isolated vertex circuit

    // Build an auxiliary adjacency with edge IDs to mark edges as used once.
    // Each undirected edge gets one id and appears once in each endpoint list
    struct Next {
        int dest;
        int id;
    };

    std::vector<std::vector<Next>> graph(num_of_vertex);
    int k = 0;
    for (int u = 0; u < num_of_vertex; ++u) {
        for (const auto& edge : adj_list[u]) {
            if (u <= edge.dest) { // assign id only once per undirected edge (handles parallel edges too)
                int id = k++;
                graph[u].push_back({edge.dest, id});
                graph[edge.dest].push_back({u, id});
            }
        }
    }
    std::vector<char> used(k, false);

    // Hierholzer's algorithm from the starting vertex 
    std::vector<int> circuit;
    std::stack<int> stck;
    stck.push(a);

    // For efficiency, we'll lazily skip used edges by popping from the back until we find an unused one.
    while (!stck.empty()) {
        int v = stck.top();
        auto &lst = graph[v];// Get the list of edges for the current vertex

        // Remove already-used edges from the tail(end) of the list
        while (!lst.empty() && used[lst.back().id]) lst.pop_back();

        // If there are still edges left, take the last one
        if (!lst.empty()) {
            auto e = lst.back();
            lst.pop_back();
            if (!used[e.id]) {
                used[e.id] = true;
                stck.push(e.dest);
            }
        } else {
            // dead end -> add to circuit and backtrack
            circuit.push_back(v);//adding the last vertex to complete the circuit
            stck.pop();
        }
    }

    std::reverse(circuit.begin(), circuit.end());// Hierholzer constructs the circuit in reverse order so we need to reverse the circuit to get the correct order
    return circuit;
}

//for the algserver 

//function to find the minimum spanning tree weight
// long long Graph::mst_weight() const {
//     return mst_weight_kruskal(*this);
// }

// int Graph::max_flow(int a, int b) const {
//     int n = num_of_vertex;
//     MaxFlow mf(n);

//     // Initialize the MaxFlow object with the graph's edges
//     for (int u = 0; u < n; ++u) {
//         for (auto &e : adj_list[u]) {
//             mf.addEdge(u, e.dest, e.weight);
//         }
//     }
//     return mf.getMaxFlow(a, b);
// }

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
  
} // namespace graph