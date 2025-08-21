#pragma once
#include "Algorithm.hpp"
#include <sstream>
//for sleep_for() and get_id()
#include <thread>
#include <chrono>
#include <iostream>

namespace graph {

struct MSTAlgorithm : Algorithm {
    std::string run(const Graph& G) override {

        // std::cerr << "[MST] running on thread " 
        //             << std::this_thread::get_id() << std::endl;

        // // using sleep to see easily the simultanously
        // std::this_thread::sleep_for(std::chrono::seconds(3));

        long long w = G.mst_weight();   // func is implement in Graph.cpp
        std::ostringstream out;
        out << "OK MST WEIGHT: " << w << "\n";
        return out.str();
    }
};

}
