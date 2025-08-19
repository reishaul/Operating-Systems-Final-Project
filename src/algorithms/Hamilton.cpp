#include "algorithms/Hamilton.hpp"
#include <functional>

#include <unordered_set>
#include <algorithm>

using namespace graph;

namespace {

// Checks if there is an edge between vertices u and v
bool hasEdge(const Graph& G, int u, int v){

    auto edges = G.get_edges(); // Get all edges in the graph
    if (u>v) std::swap(u,v);// Ensure u < v
    for (auto &t : edges){
        int a,b;//src, dest
        a = std::get<0>(t);
        b = std::get<1>(t);
        //w = std::get<2>(t);
        
        if (a==u && b==v) return true;
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
    const int n = G.get_num_of_vertex();
    if (n==0) return {};
    std::vector<int> path(n, -1), best;
    std::vector<char> used(n, false);// Track used vertices
    path[0]=0; 
    used[0]=true;

    // Depth-first search (DFS) to find Hamiltonian cycle
    std::function<bool(int)> dfs = [&](int idx)->bool{
        // Check if all vertices are included
        if (idx==n){
            if (hasEdge(G, path[n-1], path[0])) {
                best = path;
                best.push_back(path[0]); // Close the cycle
                return true;
            }
            return false;
        }
        // Try all possible next vertices
        for (int v=1; v<n; ++v){
            if (!used[v] && hasEdge(G, path[idx-1], v)){
                used[v]=true; path[idx]=v;
                if (dfs(idx+1)) return true;
                used[v]=false; path[idx]=-1;
            }
        }
        return false;
    };
    dfs(1);// Start DFS from the second vertex
    return best;// Return the best found Hamiltonian cycle
}
