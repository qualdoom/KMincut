#include <algorithm>
#include <cassert>
#include <limits>
#include <vector>

#include "stupid.hpp"

void BruteForce(
    size_t vertex, const Graph& g, size_t k, long long current_cost, std::vector<int>& color, Cut& best_cut) {
    if (current_cost >= best_cut.cost) {
        return;
    }

    if (vertex == g.n) {
        std::vector<bool> used(g.n, false);
        size_t distinct_colors{0};
        for (size_t i = 0; i < g.n; i++) {
            if (!used[color[i]]) {
                used[color[i]] = true;
                distinct_colors++;
            }
        }

        if (distinct_colors != k) {
            return;
        }

        std::vector<Edge> current_edges;

        for (const auto& edge : g.edges) {
            if (color[edge.from] != color[edge.to]) {
                current_edges.push_back(edge);
            }
        }

        best_cut.cost = current_cost;
        best_cut.k = k;
        best_cut.cut_edges = current_edges;
        return;
    }

    for (size_t where = 0; where < k; where++) {
        color[vertex] = where;
        long long next_cost = current_cost;

        for (const auto& edge : g.graph[vertex]) {
            if (edge.to < vertex && color[edge.to] != color[vertex]) {
                next_cost += edge.cost;
            }
        }

        BruteForce(vertex + 1, g, k, next_cost, color, best_cut);
    }
}

Cut GetStupidKCut(const Graph& g, size_t k) {
    Cut best_cut;
    if (g.n < k || k == 0)
        return best_cut;

    best_cut.cost = std::numeric_limits<long long>::max();

    // Cut best_cut2;
    // best_cut2.cost = std::numeric_limits<long long>::max();

    // std::vector<int> comp(g.n, 0);

    // BruteForce(0, g, k, 0, comp, best_cut2);

    // std::sort(best_cut2.cut_edges.begin(), best_cut2.cut_edges.end());
    // best_cut2.cut_edges.erase(std::unique(best_cut2.cut_edges.begin(), best_cut2.cut_edges.end()),
    //                           best_cut2.cut_edges.end());

    const size_t cntMasks = (1 << g.n);

    std::vector<std::vector<long long>> dp(cntMasks,
                                           std::vector<long long>(k + 1, std::numeric_limits<long long>::min()));

    // dp[mask][j] - min cost to get exactly k components

    dp[0][0] = 0;

    long long total_weight = 0;
    for (const auto& edge : g.edges) {
        total_weight += edge.cost;
    }

    std::vector<std::vector<long long>> matrix(g.n, std::vector<long long>(g.n, 0));
    for (size_t u = 0; u < g.n; u++) {
        for (const auto& edge : g.graph[u]) {
            matrix[edge.to][edge.from] = edge.cost;
        }
    }

    std::vector<long long> inner_weight(cntMasks, 0);
    for (size_t mask = 0; mask < cntMasks; ++mask) {
        long long current_inner = 0;
        for (const auto& edge : g.edges) {
            if (((mask >> edge.from) & 1) && ((mask >> edge.to) & 1)) {
                current_inner += edge.cost;
            }
        }
        inner_weight[mask] = current_inner;
    }

    for (size_t mask = 0; mask < cntMasks; mask++) {
        for (size_t j = 1; j <= k; j++) {
            for (int submask = mask; submask > 0; submask = (mask & (submask - 1))) {
                size_t remain = mask ^ submask;

                dp[mask][j] = std::max(dp[mask][j], dp[remain][j - 1] + inner_weight[submask]);
            }
        }
    }

    best_cut.cost = total_weight - dp[cntMasks - 1][k];
    // assert(best_cut.cost == best_cut2.cost);

    best_cut.k = k;
    best_cut.cut_edges = {};

    return best_cut;
}

Cut GetBruteForceKCut(const Graph& g, size_t k) {
    Cut best_cut2;
    if (g.n < k || k == 0)
        return best_cut2;

    best_cut2.cost = std::numeric_limits<long long>::max();

    std::vector<int> comp(g.n, 0);

    BruteForce(0, g, k, 0, comp, best_cut2);

    std::sort(best_cut2.cut_edges.begin(), best_cut2.cut_edges.end());
    best_cut2.cut_edges.erase(std::unique(best_cut2.cut_edges.begin(), best_cut2.cut_edges.end()),
                              best_cut2.cut_edges.end());

    return best_cut2;
}