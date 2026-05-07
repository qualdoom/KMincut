#pragma once

#include <cstddef>
#include <vector>

struct Edge {
    int to;
    int from;
    long long cost;
};

struct Graph {
    void Init(size_t n) {
        graph.resize(n);
    }

    void AddEdge(int u, int v, long long cost) { 
        graph[u].push_back(Edge{u, v, cost});
        graph[v].push_back({Edge{v, u, cost}});
    }

    std::vector<Edge> edges;
    std::vector<std::vector<Edge>> graph;
};