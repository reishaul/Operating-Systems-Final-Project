#pragma once
#include "Algorithm.hpp"
#include "../strategyAlg/EulerAlg.hpp"
#include "../strategyAlg/MSTAlg.hpp"
#include "../strategyAlg/HamiltonAlg.hpp"
#include "../strategyAlg/MaxFlowAlg.hpp"
#include "../strategyAlg/SCCAlg.hpp"

#include <memory>
#include <string>

namespace graph {

struct AlgorithmFactory {
    static std::unique_ptr<Algorithm> create(const std::string& name) {
        if(name == "EULER") return std::make_unique<EulerAlgorithm>();
        if(name == "MST") return std::make_unique<MSTAlgorithm>();
        if(name == "HAMILTON") return std::make_unique<HamiltonAlgorithm>();
        if(name == "MAXFLOW") return std::make_unique<MaxFlowAlgorithm>();
        if(name == "SCC") return std::make_unique<SCCAlgorithm>();
        return nullptr;
    }
};

}
