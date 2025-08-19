#include <vector>
#include <queue>
#include <algorithm>
#include <stdexcept>
#include <cstdint>

class MaxFlow {
    struct Edge {
        int to, rev;
        int cap;
    };

    int n;
    std::vector<std::vector<Edge>> adj;

public:
    explicit MaxFlow(int n) : n(n), adj(n) {}

    // Adds a directed edge from u to v with capacity cap
    void addEdge(int u, int v, int cap) {
        if (cap < 0) throw std::invalid_argument("Capacity must be non-negative");
        adj[u].push_back({v, (int)adj[v].size(), cap});
        adj[v].push_back({u, (int)adj[u].size() - 1, 0}); // reverse edge
    }

    /**
     * Computes the maximum flow from source s to sink t
     * using the Edmonds-Karp algorithm (BFS-based Ford-Fulkerson).
     * @param s Source vertex
     * @param t Sink vertex
     * @return Maximum flow from source s to sink t
     */
    int getMaxFlow(int s, int t) {
        int flow = 0;
        std::vector<int> level(n), parent(n), parentEdge(n);
        // BFS to find augmenting path
        while (true) {
            std::fill(level.begin(), level.end(), -1);// Reset level
            std::queue<int> q;
            q.push(s);
            level[s] = 0;

            // BFS to find augmenting path
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (size_t i = 0; i < adj[u].size(); ++i) {
                    Edge &e = adj[u][i];
                    if (level[e.to] < 0 && e.cap > 0) {
                        level[e.to] = level[u] + 1;
                        parent[e.to] = u;
                        parentEdge[e.to] = i;// Store the edge index
                        q.push(e.to);
                    }
                }
            }

            if (level[t] < 0) break; // no more augmenting paths

            // Find minimum capacity along the path
            int aug = INT32_MAX;
            // Find the minimum capacity along the path
            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                aug = std::min(aug, adj[u][parentEdge[v]].cap);
            }

            // Augment the flow along the path
            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                Edge &e = adj[u][parentEdge[v]];
                e.cap -= aug;
                adj[e.to][e.rev].cap += aug;// Update reverse edge capacity
            }

            flow += aug;// Update total flow
        }

        return flow;
    }
};
