#pragma once
#include "Algorithm.hpp"
#include "algorithms/MaxClique.hpp"
#include <sstream>
//for sleep_for() and get_id()
#include <thread>
#include <chrono>
#include <iostream>

namespace graph {

struct MaxCliqueAlgorithm : Algorithm {
    // implement the run method
    std::string run(const Graph& G) override {

        // std::cerr << "[MAX CLIQUE] running on thread " 
        //             << std::this_thread::get_id() << std::endl;

        // // using sleep to see easily the simultanously
        // std::this_thread::sleep_for(std::chrono::seconds(3));

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
