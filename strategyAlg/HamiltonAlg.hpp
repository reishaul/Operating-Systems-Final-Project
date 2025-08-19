#pragma once
#include "Algorithm.hpp"
#include <sstream>

namespace graph {

struct HamiltonAlgorithm : Algorithm {
    std::string run(const Graph& G) override {
        auto path = G.hamiltonian_cycle(); //func is implement in Graph.cpp
        if (path.empty()) return "ERR NO HAMILTONIAN CYCLE\n";
        std::ostringstream out;
        out << "OK";
        for (int v : path) out << " " << v;
        out << "\n";
        return out.str();

    }
};

}
