#pragma once
#include "Algorithm.hpp"
#include <sstream>

namespace graph {

struct MaxFlowAlgorithm : Algorithm {
    std::string run(const Graph& G) override {
        int flow = G.max_flow(0, G.get_num_of_vertex() - 1); // func is implement in Graph.cpp
        std::ostringstream out;
        out << "OK MAXFLOW " << flow << "\n";
        return out.str();
    }
};

}
