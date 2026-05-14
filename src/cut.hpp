#pragma once

#include <vector>

#include "graph.hpp"

struct Cut {
    size_t k;
    long long cost{0};
    std::vector<Edge> cut_edges;
};