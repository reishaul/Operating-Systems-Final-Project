#pragma once
#include "Algorithm.hpp"
#include <sstream>

namespace graph {

struct SCCAlgorithm : Algorithm {
    std::string run(const Graph& G) override {
        auto comps = G.strongly_connected_components();
        std::ostringstream ss;
        ss << "OK " << comps.size() << " COMPONENTS\n";
        for (size_t i = 0; i < comps.size(); ++i) {
            ss << "COMP " << i << ":";
            for (int v : comps[i]) ss << " " << v;
            ss << "\n";
        }
        return ss.str();
    }
};

}
