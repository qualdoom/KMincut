#pragma once

#include <vector>

#include "graph.hpp"

struct Cut {
    size_t k;
    long long cost;
    std::vector<Edge> cut_edges;
};