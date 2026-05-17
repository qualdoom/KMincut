#pragma once

#include "graph.hpp"
#include "cut.hpp"

Cut GetStupidKCut(const Graph&g, size_t k);

Cut GetBruteForceKCut(const Graph&g, size_t k);