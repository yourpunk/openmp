#pragma once

#include "vector_sum.h"

#include <optional>
#include <chrono>
#include <omp.h>

/**
 *  Data structure that stores execution times of various implementations
 */
struct Results {
    using Result = std::optional<std::chrono::nanoseconds>;

    Result time_ref;         // Execution time of the sequential implementation
    Result time_per_vector;  // Time for vector-level parallelism
    Result time_shuffle;     // Time for shuffled static scheduling
    Result time_omp_dynamic; // Time for OpenMP with dynamic scheduling
    Result time_omp_static;  // Time for OpenMP with static scheduling
};

namespace executor {

    /**
     * Finds the size of the smallest vector in the dataset.
     * If the dataset is small (less than threads * 1000), the search is done sequentially.
     */
    [[nodiscard]] inline size_t size_of_smallest_vector(const InputVectors& data) {
        size_t min_size = std::numeric_limits<size_t>::max();

        // Parallel reduction to find the minimum vector size
        #pragma omp parallel for reduction(min : min_size) \
                default(none) shared(data) if(data.size() > (size_t)omp_get_max_threads() * 1000)
        for (auto& vec : data) {
            if (vec.size() < min_size) {
                min_size = vec.size();
            }
        }
        return min_size;
    }

    /**
     * Executes a single summing method.
     * If the result is correct, returns execution time.
     * If the result is incorrect or throws an exception, returns std::nullopt.
     */
    inline Results::Result
    execute_method(SolutionFn functionPtr, const OutputVector& correct_solution, const InputVectors& data,
                   size_t minVectorSize) {
        OutputVector result(data.size());

        // Start time
        auto begin = std::chrono::steady_clock::now();

        // Execute the method
        try {
            (*functionPtr)(data, result, minVectorSize);
        } catch (...) {
            // Fail silently
        }

        // End time
        auto end = std::chrono::steady_clock::now();

        // Check correctness
        if (result == correct_solution) {
            return end - begin; // Return elapsed time
        }

        return std::nullopt;
    }

    /**
     * Overload for methods that don't need explicit minVectorSize
     */
    inline Results::Result
    execute_method(SolutionFn functionPtr, const OutputVector& correct_solution, const InputVectors& data) {
        return execute_method(functionPtr, correct_solution, data, size_of_smallest_vector(data));
    }

    /**
     * Executes all available methods and returns a Results struct with timing data
     */
    inline Results execute_methods(const OutputVector& solution, const InputVectors& data) {
        size_t shortestVectorLength = size_of_smallest_vector(data);
        auto referenceTime = execute_method(&vector_sum_sequential, solution, data, shortestVectorLength);
        auto perVectorTime = execute_method(&vector_sum_omp_per_vector, solution, data, shortestVectorLength);
        auto withShuffleTime = execute_method(&vector_sum_omp_shuffle, solution, data, shortestVectorLength);
        auto dynamicSchedTime = execute_method(&vector_sum_omp_dynamic, solution, data, shortestVectorLength);
        auto staticSchedulingTime = execute_method(&vector_sum_omp_static, solution, data, shortestVectorLength);
        return {referenceTime, perVectorTime, withShuffleTime, dynamicSchedTime, staticSchedulingTime};
    }
}
