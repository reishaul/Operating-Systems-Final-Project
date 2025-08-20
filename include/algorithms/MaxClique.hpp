#pragma once
#include "Graph.hpp"
#include <vector>

namespace graph {

// מחזיר את הקליקה המקסימלית שמצאנו בגרף
std::vector<int> find_max_clique(const Graph& G);

}
