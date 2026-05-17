#include <chrono>

#include "generators.hpp"
#include "graph.hpp"

Graph GenerateSpecialCycleGraph(size_t k) {
    std::mt19937_64 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    size_t n = 2 * k;
    Graph g;
    g.Init(n);
    g.n = n;
    for (size_t i = 0; i < k; i++) {
        g.AddEdge(i, (i + 1) % k, 1000);

        g.AddEdge(i, k + i, 1999);
    }

    return g;
}

Graph GenerateRandomGraph(size_t n, long long MaxWeight = 1000, double p = 0.5) {
    std::mt19937_64 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    Graph g;
    g.Init(n);
    g.n = n;

    const size_t kEnough = 6767676767;

    for (size_t i = 0; i < n; i++) {
        for (size_t j = i + 1; j < n; j++) {
            if ((rng() % kEnough) <= static_cast<size_t>(p * kEnough)) {
                g.AddEdge(i, j, rng() % MaxWeight + 1);
            }
        }
    }

    return g;
}

Graph GenerateConnectedRandomGraph(size_t n, long long MaxWeight = 1000, double p = 0.5) {
    std::mt19937_64 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    Graph g;
    g.Init(n);
    g.n = n;

    const size_t kEnough = 6767676767;

    for (size_t i = 0; i < n; i++) {
        g.AddEdge(i, (i + 1) % n, 2 * MaxWeight * MaxWeight);
        for (size_t j = i + 2; j < n; j++) {
            if ((rng() % kEnough) <= static_cast<size_t>(p * kEnough)) {
                g.AddEdge(i, j, (rng() % 2 + 1) * MaxWeight + rng() % 10);
            }
        }
    }

    return g;
}


std::pair<Graph, long long> GenerateClusterGraph(size_t n, size_t k, double p = 0.3) {
    std::mt19937_64 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    Graph g;
    g.Init(n);
    g.n = n;

    std::vector<int> cluster_id(n);
    for (size_t i = 0; i < n; i++) {
        cluster_id[i] = i % k;
    }

    const size_t kEnough = 6767676767;

    for (size_t i = 0; i < n; i++) {
        if (i + k < n) {
            g.AddEdge(i, i + k, 10000);
        }
        for (size_t j = i + 1; j < n; j++) {
            if (j == i + k) {
                continue;
            }
            if (cluster_id[i] == cluster_id[j]) {
                if (rng() % kEnough <= static_cast<size_t>(p * kEnough)) {
                    g.AddEdge(i, j, 10000 + (rng() % 1000));
                }
            }
        }
    }

    long long known_cost = 0;
    for (size_t c = 1; c < k; c++) {
        size_t u = c;
        size_t v = c - 1;
        long long weight = 1 + (rng() % 10);
        g.AddEdge(u, v, weight);
        known_cost += weight;
    }

    return {g, known_cost};
}

std::pair<Graph, long long> GeneratePathGraph(size_t n, size_t k) {
    std::mt19937_64 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    Graph g;
    g.Init(n);
    g.n = n;

    std::vector<long long> weights;
    for (size_t i = 0; i < n - 1; i++) {
        long long w = 1 + (rng() % 10000);
        g.AddEdge(i, i + 1, w);
        weights.push_back(w);
    }

    std::sort(weights.begin(), weights.end());
    long long known_cost = 0;
    for (size_t i = 0; i < k - 1; i++) {
        known_cost += weights[i];
    }

    return {g, known_cost};
}

std::pair<Graph, long long> GenerateCliqueCycle(size_t n, size_t k) {
    std::mt19937_64 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    Graph g;
    g.Init(n);
    g.n = n;

    long long W = 100000;
    long long clique_W = 2 * W;
    long long delta = 100;

    size_t current = 0;
    size_t gap = n / k;

    size_t start = 0;

    for (size_t c = 0; c < k; c++) {
        size_t end = std::min(start + gap, n);
        if (c < n % k) {
            end++;
        }
        g.AddEdge(start, start + 1, W + rng() % W);

        for (size_t i = start + 1; i < end; i++) {
            for (size_t j = i + 1; j < end; j++) {
                g.AddEdge(i, j, rng() % delta + clique_W);
            }
        }

        size_t next_start = end % n;
        g.AddEdge(start, next_start, W);
        start = end;
    }

    long long known_cost = k * W;

    return {g, known_cost};
}