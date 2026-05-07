#pragma once

#include "graph.hpp"

#include <cstddef>
#include <limits>
#include <queue>
#include <vector>

struct MaxFlowEdge {
    int to;
    int rev_index;
    int edge_index;
    long long initial_cap;
    long long cap;

    MaxFlowEdge(int to, int rev_index, int edge_index, long long initial_cap, long long cap)
        : to(to), rev_index(rev_index), edge_index(edge_index), initial_cap(initial_cap), cap(cap) {}
};

const long long cInf = std::numeric_limits<long long>::max();

struct MaxFlowGraphInfo {
    int n;
    int m;
    std::vector<std::vector<MaxFlowEdge>> graph;

    void Init(int v, int e) {
        n = v;
        m = e;
        graph.resize(n);
    }

    void AddEdge(int u, int v, int index, long long cap) {
        graph[u].push_back(MaxFlowEdge(v, graph[v].size(), index, cap, cap));
        graph[v].push_back(MaxFlowEdge(u, static_cast<int>(graph[u].size()) - 1, -1, 0, 0));
    }

    void AddUndirectedEdge(int u, int v, int index, long long cap) {
        AddEdge(u, v, index, cap);
        AddEdge(v, u, index, cap);
    }
};

struct Dinic {
    bool Bfs(long long to_push) {
        dist.assign(n, -1);
        std::queue<int> q;
        q.push(start_v);
        dist[start_v] = 0;

        while (!q.empty()) {
            auto u = q.front();
            q.pop();

            for (const MaxFlowEdge& e : graph[u]) {
                if (dist[e.to] == -1 && to_push <= e.cap) {
                    dist[e.to] = dist[u] + 1;
                    q.push(e.to);

                    if (e.to == final_v) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    long long Dfs(int u, long long flow, long long to_push) {
        if (u == final_v || flow == 0) {
            return flow;
        }
        for (; ptr[u] < graph[u].size(); ptr[u]++) {
            MaxFlowEdge& e = graph[u][ptr[u]];
            if (dist[e.to] == dist[u] + 1 && e.cap >= to_push) {
                long long min_flow = std::min(flow, e.cap);
                long long pushed = Dfs(e.to, min_flow, to_push);

                if (pushed > 0) {
                    // found flow
                    e.cap -= pushed;
                    graph[e.to][e.rev_index].cap += pushed;
                    return pushed;
                }
            }
        }

        return 0;
    }

    void Init(MaxFlowGraphInfo& g, int s, int t) {
        graph = g.graph;
        n = g.n;
        start_v = s;
        final_v = t;
    }

    long long GetFlow(int max_two_power) {
        long long flow = 0;

        for (; max_two_power >= 0; max_two_power--) {
            long long to_push = (1LL << max_two_power);

            while (Bfs(to_push)) {
                ptr.assign(n, 0);
                long long pushed = cInf;
                do {
                    pushed = Dfs(start_v, cInf, to_push);
                    flow += pushed;
                } while (pushed != 0);
            }
        }

        return flow;
    }

    int n;
    std::vector<std::vector<MaxFlowEdge>> graph;
    std::vector<int> dist;
    std::vector<size_t> ptr;
    int start_v;
    int final_v;
};

struct GomoryHuTree {
    

    Graph g;

};