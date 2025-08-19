#pragma once
#include "Algorithm.hpp"
#include <sstream>

namespace graph {

struct EulerAlgorithm : Algorithm {
    std::string run(const Graph& G) override {
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
