#pragma once
#include "Algorithm.hpp"
#include "../strategyAlg/MSTAlg.hpp"
#include "../strategyAlg/HamiltonAlg.hpp"
#include "../strategyAlg/MaxFlowAlg.hpp"
#include "../strategyAlg/MaxCliqueAlg.hpp" // Include the MaxClique algorithm

#include <memory>
#include <string>

namespace graph {

// Factory class for creating algorithm instances
struct AlgorithmFactory {
    static std::unique_ptr<Algorithm> create(const std::string& name) {
        if(name == "MST") return std::make_unique<MSTAlgorithm>();
        if(name == "HAMILTON") return std::make_unique<HamiltonAlgorithm>();
        if(name == "MAXFLOW") return std::make_unique<MaxFlowAlgorithm>();
        if(name=="MAXCLIQUE") return std::make_unique<MaxCliqueAlgorithm>();
        return nullptr;
    }
};

}
