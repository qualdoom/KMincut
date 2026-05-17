#pragma once

#include "graph.hpp"
#include "cut.hpp"

Cut GetGreedyKCut(const Graph&g, size_t k);