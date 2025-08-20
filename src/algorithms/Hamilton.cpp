#include "algorithms/Hamilton.hpp"
#include <functional>

#include <unordered_set>
#include <algorithm>

using namespace graph;

namespace {

// Checks if there is an edge between vertices u and v
bool hasEdge(const Graph& G, int u, int v){

    auto edges = G.get_edges(); // Get all edges in the graph
    //if (u>v) std::swap(u,v);// Ensure u < v
    for (auto &e : edges){
        int a,b;//src, dest
        a = std::get<0>(e);
        b = std::get<1>(e);
        
        if ((a==u && b==v) || (a==v &&b==u)) return true;
    }
    return false;
}
}

/**
 * Finds a Hamiltonian cycle in the given graph.
 * @param G The input graph
 * @return A vector containing the vertices in the Hamiltonian cycle, or an empty vector if no such cycle exists
 */
std::vector<int> find_hamiltonian_cycle(const Graph& G){
    int n = G.get_num_of_vertex();
    if (n<=1) return {};

    std::vector<int> path(n, -1), best;
    std::vector<char> used(n, false);// Track used vertices
    std::vector<int> res;

    // Depth-first search (DFS) to find Hamiltonian cycle
    std::function<bool(int)> dfs = [&](int index)->bool{
        // Check if all vertices are included
        if (index==n){
            if (hasEdge(G, path[n-1], path[0])) {
                res = path;
                res.push_back(path[0]); // Close the cycle
                return true;
            }
            return false;
        }
        // Try all possible next vertices
        for (int v=0; v<n; ++v){
            if (!used[v] &&(index==0 || hasEdge(G, path[index-1], v))){
                path[index]=v;
                used[v]=true;

                if (dfs(index+1)) return true;
                path[index]=-1;

                used[v]=false;
            }
        }
        return false;
    };
    dfs(0);// Start DFS from the second vertex
    return res;// Return the best found Hamiltonian cycle
}
