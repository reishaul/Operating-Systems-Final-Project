#include "algorithms/MST.hpp"
#include "Graph.hpp"
#include <algorithm>

/**
 * implement Kruskal's algorithm for finding the minimum spanning tree (MST) of a graph
 */
namespace {

struct DSU {// Disjoint Set Union (DSU) data structure
    std::vector<int> p, r;//p[i] = parent of i, r[i] = rank of i

    explicit DSU(int n): p(n), r(n,0) { for (int i=0;i<n;++i) p[i]=i; }//constructor

    int find(int x){ return p[x]==x?x:p[x]=find(p[x]); }//find return the candidate of the set

    bool unite(int a,int b){//union the sets that contain a and b 
        a=find(a); b=find(b);
        if(a==b) return false;
        if(r[a]<r[b]) std::swap(a,b);
        p[b]=a;
        if(r[a]==r[b]) ++r[a];
        return true;
    }
};
}

/** 
 * Kruskal's algorithm for finding the minimum spanning tree (MST) of a graph
 */
long long mst_weight_kruskal(const graph::Graph& G){
    const int n = G.get_num_of_vertex();
    auto edges = G.get_edges(); 

    std::vector<std::tuple<int,int,int>> E(edges.begin(), edges.end());// Convert edges to a vector of tuples (src, dest, weight)

    std::sort(E.begin(), E.end(),
              [](auto &a, auto &b){ return std::get<2>(a) < std::get<2>(b); });//sort the edges by weight

    DSU dsu(n);// Disjoint Set Union (DSU) for Kruskal's algorithm
    long long total = 0;
    int used = 0;
    for (auto &e : E) {// Iterate over the edges
        int u,v,w; 
        std::tie(u,v,w)=e;// Extract the edge information
        if (dsu.unite(u,v)) {
            total += w; ++used; if (used == n-1) break;
        }
    }
    // If the graph is not connected, there is no "true" MST; return the sum of the minimum spanning forest
    return total;
}
