#pragma once
#include "Algorithm.hpp"
#include "algorithms/MaxClique.hpp"
#include <sstream>
#include <thread>
#include <chrono>
#include <iostream>

namespace graph {

struct MaxCliqueAlgorithm : Algorithm {
    // implement the run method
    std::string run(const Graph& G) override {

        auto clique = find_max_clique(G);
        if (clique.empty()) return "ERR NO CLIQUE\n";

        std::ostringstream out;//for output
        out << "OK MAX CLIQUE SIZE: " << clique.size();
        out << " CLIQUE MEMBERS:";
        for (auto v : clique) out << " " << v;
        out << "\n";
        return out.str();
    }
};

}
