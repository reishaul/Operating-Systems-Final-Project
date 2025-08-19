#pragma once
#include "Graph.hpp"
#include <string>

namespace graph {

struct Algorithm {
    virtual ~Algorithm() = default;
    virtual std::string run(const Graph& G) = 0;
};

}
