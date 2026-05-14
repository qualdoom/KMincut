#pragma once

#include "dinic.hpp"
#include "graph.hpp"

struct GomoryHuTree {
    Graph tree;

    void Build(const Graph& g) {
        origin = g;
        size_t n = g.n;
        std::vector<int> parent(n, 0);
        tree.Init(n);

        for (size_t i = 1; i < n; i++) {
            int s = i;
            int t = parent[i];

            MaxFlowGraphInfo h;
            h.Init(n, 0);

            for (const auto& edge : g.edges) {
                h.AddUndirectedEdge(edge.to, edge.from, -1, edge.cost);
            }

            Dinic dinic;
            dinic.Init(h, s, t);

            auto flow = dinic.GetFlow(/*max_two_power=*/60);
            dinic.Bfs(1);  // for finding achievable vertices

            for (int j = i + 1; j < n; j++) {
                if (parent[j] == t && dinic.dist[j] != -1) {
                    parent[j] = s;
                }
            }

            tree.AddEdge(parent[i], i, flow);
        }
    }

    Graph origin;
};