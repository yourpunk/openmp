/**
 * =============================
 * Vector Summing Benchmark Tool
 * =============================
 *
 * This program benchmarks multiple implementations of vector summing.
 * Each implementation uses a different parallelization strategy:
 *  - Sequential (single-threaded)
 *  - Vector-level multithreading (one thread per vector)
 *  - Shuffling + static scheduling
 *  - OpenMP with dynamic scheduling
 *  - OpenMP with static scheduling
 *
 * The goal is to compare how each strategy performs on datasets with different characteristics:
 *  - Few very long vectors
 *  - Highly diverse vector lengths
 *  - Millions of very short vectors
 *  - Small dataset unsuitable for parallelism
 *
 * Results are printed in a formatted table.
 */

#include "vector_sum.h"
#include "data_generator.h"
#include "executor.h"
#include "TextTable.h"

#include <string>
#include <ios>
#include <iomanip>
#include <chrono>

/**
 * Converts a double to a fixed-precision string
 */
std::string to_string(double x, int precision) {
    std::stringstream s;
    s << std::fixed << std::setprecision(precision) << x;
    return s.str();
}

/**
 * Converts optional nanoseconds duration to a human-readable time string (e.g. "1.45 ms")
 */
std::string to_string(std::optional<std::chrono::nanoseconds> test_time) {
    if (!test_time) {
        return "!!!"; // Missing value
    }

    auto ns = test_time->count();
    if (ns >= 1'000'000'000) return to_string((double)ns / 1e9, 2) + " s";
    else if (ns >= 1'000'000) return to_string((double)ns / 1e6, 2) + " ms";
    else if (ns >= 1'000) return to_string((double)ns / 1e3, 2) + " us";
    else return std::to_string(ns) + " ns";
}

/**
 * Adds a row with test results to the output table
 */
void add_table_row(const std::string& label, const Results& results, TextTable& table) {
    table.add(label);
    table.add(to_string(results.time_ref));
    table.add(to_string(results.time_per_vector));
    table.add(to_string(results.time_shuffle));
    table.add(to_string(results.time_omp_dynamic));
    table.add(to_string(results.time_omp_static));
    table.endOfRow();
}

/**
 * Dataset 0:
 * Few extremely long vectors — tests memory and processing under heavy load per vector.
 */
void test_dataset_0(TextTable& table) {
    InputVectors data(3, std::vector<int8_t>(250'000'000));
    OutputVector solution(data.size());
    data_generator::generate_data(solution, data);

    auto results = executor::execute_methods(solution, data);
    add_table_row("Few very long vectors", results, table);
}

/**
 * Dataset 1:
 * Mixed sizes — some vectors are huge, others very short.
 * Designed to test load imbalance and scheduler performance.
 */
void test_dataset_1(TextTable& table) {
    std::array<size_t, 8192> lengths{};

    auto large_data_count = lengths.size() / 8;
    auto split_it = lengths.begin() + static_cast<ptrdiff_t>(large_data_count);

    data_generator::generate_vector_sizes(lengths.begin(), split_it, 500'000, 300'000); // long vectors
    data_generator::generate_vector_sizes(split_it, lengths.end(), 5, 3);               // short vectors

    InputVectors data;
    for (auto j : lengths) {
        data.push_back(std::vector<int8_t>(j));
    }

    OutputVector solution(data.size());
    data_generator::generate_data(solution, data);

    auto results = executor::execute_methods(solution, data);
    add_table_row("High variance in vector lengths", results, table);
}

/**
 * Dataset 2:
 * Many short vectors — tests overhead of thread scheduling and small workload distribution.
 */
void test_dataset_2(TextTable& table) {
    InputVectors data(10'000'000, std::vector<int8_t>(2));
    OutputVector solution(data.size());
    data_generator::generate_data(solution, data);

    auto results = executor::execute_methods(solution, data);
    add_table_row("Many short vectors", results, table);
}

/**
 * Dataset 3:
 * Tiny dataset — not worth parallelizing, used to measure overhead of parallel implementations.
 */
void test_dataset_3(TextTable& table) {
    InputVectors data(10, std::vector<int8_t>(10));
    OutputVector solution(data.size());
    data_generator::generate_data(solution, data);

    auto results = executor::execute_methods(solution, data);
    add_table_row("Too small for parallelism", results, table);
}

/**
 * Main entry point — prepares table, runs all test cases, and prints results.
 */
int main() {
    TextTable table{};

    // Header row
    table.add("");
    table.add("Sequential");
    table.add("Vector-level Parallelism");
    table.add("Shuffling");
    table.add("Dynamic Scheduling");
    table.add("Static Scheduling");
    table.endOfRow();

    // Run all tests
    test_dataset_0(table);
    test_dataset_1(table);
    test_dataset_2(table);
    test_dataset_3(table);

    // Align first column
    table.setAlignment(0, TextTable::Alignment::RIGHT);

    // Output the result table
    std::cout << table;

    return 0;
}
