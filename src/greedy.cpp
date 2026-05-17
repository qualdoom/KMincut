#include <algorithm>
#include <numeric>
#include <vector>
#include "greedy.hpp"

struct DSU {

    void Init(size_t other_n) {
        n = other_n;
        d.assign(n, 1);
        p.assign(n, 0);
        std::iota(p.begin(), p.end(), 0);
    }

    int Find(int u) {
        if (u == p[u]) return u;
        return p[u] = Find(p[u]);
    }

    void Unite(int u, int v) {
        u = Find(u);
        v = Find(v);

        if (u == v) {
            return;
        }

        if (d[u] < d[v]) {
            std::swap(u, v);
        }

        d[u] += d[v];
        p[v] = u;
    }

    std::vector<int> d;
    std::vector<int> p;
    size_t n;
};

Cut GetGreedyKCut(const Graph&g, size_t k) {
    Cut cut;

    std::vector<Edge> edges = g.edges;

    std::sort(edges.rbegin(), edges.rend()); // reversed sort

    DSU d;
    d.Init(g.n);

    size_t components = g.n;

    size_t cost = 0;
    size_t total = 0;

    for (const auto& edge: edges) {
        total += edge.cost;

        if (d.Find(edge.to) == d.Find(edge.from)) {
            cost += edge.cost; 
        } else {
            if (components != k) {
                d.Unite(edge.to, edge.from);
                cost += edge.cost;
                components--;
            }
        }
    }

    cut.cost = total - cost;

    return cut;
}