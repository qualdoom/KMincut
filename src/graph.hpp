#pragma once

#include <compare>
#include <cstddef>
#include <iostream>
#include <vector>

struct Edge {
    int to;
    int from;
    long long cost;
    int edge_number;

    bool operator==(const Edge& another) const = default;

    std::strong_ordering operator<=>(const Edge& another) const {
        if (cost != another.cost) {
            return cost <=> another.cost;
        }

        if (to != another.to) {
            return to <=> another.to;
        }

        if (from != another.from) {
            return from <=> another.from;
        }

        return edge_number <=> another.edge_number;
    }
};

struct Graph {
    void Init(size_t n) { graph.resize(n); }

    void ReadGraph(std::istream& is, size_t n, size_t cnt_edges) {
        this->n = n;
        Init(n);
        for (size_t j = 0; j < cnt_edges; j++) {
            size_t u;
            size_t v;
            long long cost;
            is >> u >> v >> cost;
            u--;v--;
            AddEdge(u, v, cost);
        }
    }

    void AddEdge(int u, int v, long long cost) {
        graph[u].push_back(Edge{v, u, cost, edge_number});
        graph[v].push_back({Edge{u, v, cost, edge_number}});
        edges.push_back(Edge{u, v, cost, edge_number++});
    }

    int edge_number{0};
    size_t n;
    std::vector<Edge> edges;
    std::vector<std::vector<Edge>> graph;
};