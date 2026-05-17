#include <chrono>
#include <fstream>
#include <iostream>

#include "generators.hpp"
#include "graph.hpp"
#include "greedy.hpp"
#include "minkcut_2_factor.hpp"
#include "stupid.hpp"

#include <cassert>

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
        std::cerr << "Usage: " << argv[0] << "path_to_directory_where_to_save_csv\n";
        return 1;
    }

    std::string out_path = argv[1];

    out_path += "/benchmark_random_results.csv";

    std::ofstream csv(out_path);

    if (!csv.is_open()) {
        std::cerr << "Could not open file for writing\n";
        return 1;
    }

    static constexpr size_t kSamples = 25;

    csv << "graph_type,n,k,approx_cost,approx_time_ms,greedy_cost,greedy_time_ms,exact_cost,exact_time_ms\n";

    std::cout << "Starting benchmarks...\n";

    for (size_t n = 2; n <= 16; n++) {
        size_t max_k = 10;
        for (size_t k = 2; k <= std::min(n, max_k); k++) {
            for (size_t samples = 0; samples < kSamples; samples++) {
                Graph g = GenerateRandomGraph(n, 1000, 0.5);

                long long approx_cost = 0;
                double approx_time = MeasureTime(GetApproxKCut, g, k, approx_cost);

                long long greedy_cost = 0;

                double greedy_time = MeasureTime(GetGreedyKCut, g, k, greedy_cost);

                long long exact_cost = 0;

                double exact_time = MeasureTime(GetStupidKCut, g, k, exact_cost);

                assert(greedy_cost >= exact_cost);

                csv << "random," << n << "," << k << "," << approx_cost << "," << approx_time << "," << greedy_cost << "," << greedy_time << "," << exact_cost
                    << "," << exact_time << std::endl;
                std::cout << "random," << n << "," << k << "," << approx_cost << "," << approx_time << "," << exact_cost
                          << "," << exact_time << '\n';
            }
        }
    }
}