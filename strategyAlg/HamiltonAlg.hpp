#pragma once
#include "Algorithm.hpp"
#include "algorithms/Hamilton.hpp"
#include <sstream>
//for sleep_for() and get_id()
#include <thread>
#include <chrono>
#include <iostream>

namespace graph {

struct HamiltonAlgorithm : Algorithm {
    std::string run(const Graph& G) override {
        // std::cerr << "[Hamilton] running on thread " 
        //             << std::this_thread::get_id() << std::endl;

        // // using sleep to see easily the simultanously
        // std::this_thread::sleep_for(std::chrono::seconds(3));

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
