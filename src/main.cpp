#include "minkcut_2_factor.hpp"

#include <iostream>

int main() {
    Graph g;

    size_t n, m, k;
    std::cin >> n >> m >> k;
    g.ReadGraph(std::cin, n, m);

    auto cut = GetApproxKCut(g, k);

    std::cout << cut.cost << std::endl;
}