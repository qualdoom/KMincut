#include <gtest/gtest.h>
#include "minkcut_2_factor.hpp"
#include "graph.hpp"

TEST(KMinCutTest, PathGraph) {
    Graph g;
    g.Init(3);
    g.AddEdge(0, 1, 10);
    g.AddEdge(1, 2, 20);

    auto cut = GetApproxKCut(g, 2);

    EXPECT_LE(cut.cost, 10 * 2);
}

TEST(KMinCutTest, DumbbellGraph) {
    Graph g;
    g.Init(4);
    g.AddEdge(0, 1, 100);
    g.AddEdge(1, 2, 1);
    g.AddEdge(2, 3, 100);

    auto cut = GetApproxKCut(g, 2);

    EXPECT_LE(cut.cost, 1 * 2);
}

TEST(KMinCutTest, DumbbellGraph) {
    Graph g;
    g.Init(4);
    g.AddEdge(0, 1, 100);
    g.AddEdge(1, 2, 1);
    g.AddEdge(2, 3, 100);

    auto cut = GetApproxKCut(g, 2);

    EXPECT_LE(cut.cost, 1 * 2);
}