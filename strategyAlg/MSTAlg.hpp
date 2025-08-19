#pragma once
#include "Algorithm.hpp"
#include <sstream>

namespace graph {

struct MSTAlgorithm : Algorithm {
    std::string run(const Graph& G) override {
        long long w = G.mst_weight();   // func is implement in Graph.cpp
        std::ostringstream out;
        out << "OK MST WEIGHT: " << w << "\n";
        return out.str();
    }
};

}
