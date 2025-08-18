#include "SCC.hpp"
#include <stack>
#include <functional>

/**
 * Finds the strongly connected components of a directed graph using Kosaraju's algorithm.
 * @param G Directed graph represented as a Digraph object
 * @return A vector of strongly connected components, where each component is represented as a vector of node indices.
 */
std::vector<std::vector<int>> kosaraju_scc(const Digraph& G){
    int n = G.size();
    std::vector<char> visited(n,false);//vector to track visited nodes
    std::vector<int> order;//vector to store the finishing order
    order.reserve(n);//reserve space for finishing order

    // 1)run DFS on the original graph to build the finishing order
    std::function<void(int)> dfs1 = [&](int v){
        visited[v]=true;
        for(int to: G.adj(v)) if(!visited[to]) dfs1(to);
        order.push_back(v);
    };
    for(int i=0;i<n;++i) if(!visited[i]) dfs1(i);

    // 2) build the transpose graph
    std::vector<std::vector<int>> gr(n);
    for(int v=0; v<n; ++v) for(int to: G.adj(v)) gr[to].push_back(v);

    // 3) DFS in reverse order
    std::fill(visited.begin(), visited.end(), false);//reset visited for second pass
    std::vector<std::vector<int>> comps;//vector to store the strongly connected components
    for(int i=n-1;i>=0;--i){
        int v = order[i];
        if (visited[v]) continue;
        std::vector<int> comp;
        std::stack<int> st;//using stack for DFS
        st.push(v);//insert the starting node
        visited[v]=true;
        while(!st.empty()){
            int x=st.top(); st.pop();
            comp.push_back(x);
            for(int to: gr[x]) if(!visited[to]){ visited[to]=true; st.push(to); }
        }
        comps.push_back(std::move(comp));//add the component to the result
    }
    return comps;
}
