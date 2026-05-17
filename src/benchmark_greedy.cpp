#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "generators.hpp"
#include "graph.hpp"
#include "greedy.hpp"
#include "minkcut_2_factor.hpp"

template <typename Func>
double MeasureTime(Func&& func, const Graph& g, size_t k, long long& out_cost) {
    auto start = std::chrono::high_resolution_clock::now();
    Cut cut = func(g, k);
    auto end = std::chrono::high_resolution_clock::now();

    out_cost = cut.cost;
    std::chrono::duration<double, std::milli> duration = end - start;
    return duration.count();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_directory_where_to_save_csv>\n";
        return 1;
    }

    std::string out_path = argv[1];
    out_path += "/benchmark_greedy_results.csv";

    std::ofstream csv(out_path);

    if (!csv.is_open()) {
        std::cerr << "Could not open file for writing\n";
        return 1;
    }

    csv << "graph_type,n,k,approx_cost,approx_time_ms,greedy_cost,greedy_time_ms,bound_cost\n";

    std::vector<size_t> large_ns = {100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600};
    std::vector<size_t> large_ks = {3, 5, 8, 10, 12, 15, 20};

    static constexpr size_t kLargeSamples = 3;

    for (size_t n : large_ns) {
        for (size_t k : large_ks) {
            if (k > n / 2)
                continue;

            for (size_t samples = 0; samples < kLargeSamples; samples++) {
                long long approx_cost = 0;
                double approx_time = 0.0;

                auto [g_cluster, bound_cluster] = GenerateClusterGraph(n, k, 0.3);
                approx_time = MeasureTime(GetApproxKCut, g_cluster, k, approx_cost);

                long long greedy_cost = 0;
                double greedy_time = MeasureTime(GetGreedyKCut, g_cluster, k, greedy_cost);

                csv << "cluster," << n << "," << k << "," << approx_cost << "," << approx_time << "," << greedy_cost
                    << "," << greedy_time << ',' << bound_cluster << '\n';

                auto [g_path, bound_path] = GeneratePathGraph(n, k);
                approx_time = MeasureTime(GetApproxKCut, g_path, k, approx_cost);

                greedy_time = MeasureTime(GetGreedyKCut, g_path, k, greedy_cost);

                csv << "path," << n << "," << k << "," << approx_cost << "," << approx_time << "," << greedy_cost << ","
                    << greedy_time << ',' << bound_path << '\n';

                auto [g_cycle, bound_cycle] = GenerateCliqueCycle(n, k);
                approx_time = MeasureTime(GetApproxKCut, g_cycle, k, approx_cost);

                greedy_time = MeasureTime(GetGreedyKCut, g_cycle, k, greedy_cost);

                csv << "clique_cycle," << n << "," << k << "," << approx_cost << "," << approx_time << ","
                    << greedy_cost << "," << greedy_time << ',' << bound_cycle << std::endl;
                
                auto g_rand = GenerateConnectedRandomGraph(n, k, 0.1);
                approx_time = MeasureTime(GetApproxKCut, g_rand, k, approx_cost);

                greedy_time = MeasureTime(GetGreedyKCut, g_rand, k, greedy_cost);

                csv << "random_0.1," << n << "," << k << "," << approx_cost << "," << approx_time << "," << greedy_cost
                    << "," << greedy_time << ",0" << std::endl;

                g_rand = GenerateConnectedRandomGraph(n, k, 0.4);
                approx_time = MeasureTime(GetApproxKCut, g_rand, k, approx_cost);

                greedy_time = MeasureTime(GetGreedyKCut, g_rand, k, greedy_cost);

                csv << "random_0.4," << n << "," << k << "," << approx_cost << "," << approx_time << "," << greedy_cost
                    << "," << greedy_time << ",0" << std::endl;

                std::cout << "Done" << n << "," << k << "," << approx_cost << "," << approx_time << "," << greedy_cost
                          << "," << greedy_time << ',' << bound_cycle << '\n';
            }
        }
    }
    return 0;
}