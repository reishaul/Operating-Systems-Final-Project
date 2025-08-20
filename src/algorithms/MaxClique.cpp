#include "algorithms/MaxClique.hpp"
#include <algorithm>

namespace graph {

static bool isNeighbor(const Graph& G, int u, int v) {
    for (auto [nbr, w] : G.neighbors(u)) {
        if (nbr == v) return true;
    }
    return false;
}

// מימוש Bron–Kerbosch
static void bronKerbosch(const Graph& G,
                         std::vector<int> R,
                         std::vector<int> P,
                         std::vector<int> X,
                         std::vector<int>& bestClique) {
    if (P.empty() && X.empty()) {
        if (R.size() > bestClique.size()) {
            bestClique = R;
        }
        return;
    }

    int u = -1;
    if (!P.empty()) u = P[0];
    else if (!X.empty()) u = X[0];

    std::vector<int> PwithoutNeighbors;
    for (int v : P) {
        if (u == -1 || !isNeighbor(G, u, v)) {
            PwithoutNeighbors.push_back(v);
        }
    }

    for (int v : PwithoutNeighbors) {
        std::vector<int> Rnext = R;
        Rnext.push_back(v);

        std::vector<int> Pnext, Xnext;
        for (auto [w, _] : G.neighbors(v)) {
            if (std::find(P.begin(), P.end(), w) != P.end())
                Pnext.push_back(w);
            if (std::find(X.begin(), X.end(), w) != X.end())
                Xnext.push_back(w);
        }

        bronKerbosch(G, Rnext, Pnext, Xnext, bestClique);

        P.erase(std::remove(P.begin(), P.end(), v), P.end());
        X.push_back(v);
    }
}

std::vector<int> find_max_clique(const Graph& G) {
    int n = G.get_num_of_vertex();
    std::vector<int> R, P(n), X, best;
    for (int i = 0; i < n; i++) P[i] = i;
    bronKerbosch(G, R, P, X, best);
    return best;
}

}
