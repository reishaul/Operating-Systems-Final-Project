#pragma once
#include "Algorithm.hpp"
#include <sstream>
//for sleep_for() and get_id()
#include <thread>
#include <chrono>
#include <iostream>

namespace graph {

struct EulerAlgorithm : Algorithm {
    std::string run(const Graph& G) override {

        // std::cerr << "[Euler] running on thread " 
        //             << std::this_thread::get_id() << std::endl;

        // // using sleep to see easily the simultanously
        // std::this_thread::sleep_for(std::chrono::seconds(3));

        if(!G.has_eulerian_cycle()) return "ERR NO_EULERIAN_CYCLE\n";
        auto circuit = G.get_eulerian_cycle();
        std::ostringstream out;
        out << "OK";
        for(int v : circuit) out << " " << v;
        out << "\n";
        return out.str();
    }
};

}
