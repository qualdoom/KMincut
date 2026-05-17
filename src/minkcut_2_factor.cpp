#include "graph.hpp"
#include "minkcut_2_factor.hpp"
#include "gomory_hu_tree.hpp"

#include <algorithm>
#include <vector>

void Dfs(int u, const Graph& tree, int color, std::vector<int>& colors, int p = -1) {
    colors[u] = color;

    for (const auto& edge: tree.graph[u]) {
        if (edge.to == p) continue;

        Dfs(edge.to, tree, color, colors, u);
    }
}

Cut GetApproxKCut(const Graph&g, size_t k) {
    GomoryHuTree tree;
    tree.Build(g);

    std::vector<Edge> tree_edges = tree.tree.edges;

    std::sort(tree_edges.begin(), tree_edges.end());

    Cut cut;

    size_t n = g.n;

    std::vector<Edge> cut_edges;

    size_t edges_to_cut = std::min(k - 1, tree_edges.size());

    for (size_t j = 0; j < edges_to_cut; j++) {
        int u = tree_edges[j].from;
        int v = tree_edges[j].to;
        
        std::vector<int> colors(n, 0);
        Dfs(u, tree.tree, 0, colors, v);
        Dfs(v, tree.tree, 1, colors, u);

        for (int t = 0; t < n; t++) {
            for (const auto& edge: g.graph[t]) {
                if (edge.to < edge.from && colors[edge.to] != colors[edge.from]) {
                    cut_edges.push_back(edge);
                }
            }
        }
    }

    sort(cut_edges.begin(), cut_edges.end());
    cut_edges.erase(std::unique(cut_edges.begin(), cut_edges.end()), cut_edges.end());

    cut.cost = 0;

    for (const auto& edge: cut_edges) {
        cut.cost += edge.cost;
    }

    cut.k = k;
    cut.cut_edges = cut_edges;

    return cut;
}