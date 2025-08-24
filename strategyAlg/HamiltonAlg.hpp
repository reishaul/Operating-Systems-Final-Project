#pragma once
#include "Algorithm.hpp"
#include "algorithms/Hamilton.hpp"
#include <sstream>
#include <thread>
#include <chrono>
#include <iostream>

namespace graph {

struct HamiltonAlgorithm : Algorithm {
    std::string run(const Graph& G) override {

        auto cycle = find_hamiltonian_cycle(G); //func is implement in Graph.cpp
        if (cycle.empty()) return "ERR NO HAMILTONIAN CYCLE\n";
        std::ostringstream out;
        out << "OK"<<" HAM VERTEX:";
        for (auto v : cycle) out << " " << v;
        out << "\n";
        return out.str();

    }
};

}
