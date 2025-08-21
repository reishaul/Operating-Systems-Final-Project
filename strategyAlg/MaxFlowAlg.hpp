#pragma once
#include "Algorithm.hpp"
#include <sstream>
//for sleep_for() and get_id()
#include <thread>
#include <chrono>
#include <iostream>

namespace graph {

struct MaxFlowAlgorithm : Algorithm {
    std::string run(const Graph& G) override {
        // std::cerr << "[MAX FLOW] running on thread " 
        //             << std::this_thread::get_id() << std::endl;

        // // using sleep to see easily the simultanously
        // std::this_thread::sleep_for(std::chrono::seconds(3));

        int flow = G.max_flow(0, G.get_num_of_vertex() - 1); // func is implement in Graph.cpp
        std::ostringstream out;
        out << "OK MAX FLOW " << flow << "\n";
        return out.str();
    }
};

}
