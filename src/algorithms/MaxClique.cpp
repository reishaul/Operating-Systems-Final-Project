#include "algorithms/MaxClique.hpp"
#include <algorithm>

namespace graph {

/**
 * @brief Checks if two vertices are neighbors in the graph.
 * @param G The graph
 * @param u The first vertex.
 * @param v The second vertex
 * @return true if u and v are neighbors, false otherwise.
 */
static bool isNeighbor(const Graph& G, int u, int v) {
    for (auto [nbr, w] : G.neighbors(u)) {
        if (nbr == v) return true;
    }
    return false;
}

/**
 * @brief Implements the Bron-Kerbosch algorithm for finding maximal cliques.
 * @param G The graph
 * @param R The current clique
 * @param P The candidates for the next vertex to add to the clique.
 * @param X The vertices that have already been considered.
 * @param bestClique The best clique found so far
 */
static void bronKerbosch(const Graph& G,
                         std::vector<int> R,
                         std::vector<int> P,
                         std::vector<int> X,
                         std::vector<int>& bestClique) {
    if (P.empty() && X.empty()) {// Base case: no more candidates or excluded vertices
        if (R.size() > bestClique.size()) {// Found a new best clique
            bestClique = R;
        }
        return;
    }

    int u = -1;// Pivot
    if (!P.empty()) u = P[0];// Choose a pivot from P
    else if (!X.empty()) u = X[0];// or from X

    std::vector<int> PwithoutNeighbors;//vector that contains all candidates not connected to the pivot
    for (int v : P) {//go over the candidates and check if they are connected to the pivot
        if (u == -1 || !isNeighbor(G, u, v)) {
            PwithoutNeighbors.push_back(v);
        }
    }

    for (int v : PwithoutNeighbors) {//go over the candidates not connected to the pivot
        std::vector<int> Rnext = R;//vector that contains the current clique plus the new vertex
        Rnext.push_back(v);//insert new vertex into the current clique

        std::vector<int> Pnext, Xnext;
        for (auto [w, _] : G.neighbors(v)) {//go over the neighbors of the new vertex
            if (std::find(P.begin(), P.end(), w) != P.end())//if w is in P
                Pnext.push_back(w);
            if (std::find(X.begin(), X.end(), w) != X.end())//if w is in X
                Xnext.push_back(w);
        }

        bronKerbosch(G, Rnext, Pnext, Xnext, bestClique);// Recursive call

        P.erase(std::remove(P.begin(), P.end(), v), P.end());// Remove v from P
        X.push_back(v);// Add v to X
    }
}

/**
 * @brief Finds the maximum clique in a graph.
 * @param G The graph
 * @return A vector containing the vertices of the maximum clique.
 */
std::vector<int> find_max_clique(const Graph& G) {
    int n = G.get_num_of_vertex();
    std::vector<int> R, P(n), X, best;
    for (int i = 0; i < n; i++) P[i] = i;//define P
    bronKerbosch(G, R, P, X, best);// Call the Bron-Kerbosch algorithm
    return best;
}

}
