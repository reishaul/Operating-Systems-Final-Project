#pragma once
#include "Algorithm.hpp"
#include "algorithms/MaxClique.hpp"
#include <sstream>

namespace graph {

struct MaxCliqueAlgorithm : Algorithm {
    std::string run(const Graph& G) override {
        auto clique = find_max_clique(G);
        if (clique.empty()) return "ERR NO CLIQUE\n";

        std::ostringstream out;
        out << "OK MAX CLIQUE SIZE: " << clique.size() << "\n";
        out << "CLIQUE MEMBERS:";
        for (auto v : clique) out << " " << v;
        out << "\n";
        return out.str();
    }
};

}
