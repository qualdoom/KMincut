#pragma once

#include <random>

#include "graph.hpp"

Graph GenerateSpecialCycleGraph(size_t n);

Graph GenerateRandomGraph(size_t n, long long MaxWeight, double p);

std::pair<Graph, long long> GenerateClusterGraph(size_t n, size_t k, double p);

std::pair<Graph, long long> GeneratePathGraph(size_t n, size_t k);

std::pair<Graph, long long> GenerateCliqueCycle(size_t n, size_t k);

Graph GenerateConnectedRandomGraph(size_t n, long long MaxWeight, double p);
