#include "vector_sum.h"
#include <omp.h>
#include <numeric>
#include <algorithm>
#include <random>
 
// Parallel sum computation within each vector
// Suitable for a small number of long vectors
void vector_sum_omp_per_vector(const InputVectors& data, OutputVector& solution, size_t min_vector_size) {
    #pragma omp parallel for
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i].size() >= min_vector_size) {
            int local_sum = 0;
            // Parallelize summation within a single vector
            // Using reduction to avoid race conditions
            #pragma omp parallel for reduction(+:local_sum) schedule(static, 256) 
            for (size_t j = 0; j < data[i].size(); ++j) {
                local_sum += data[i][j];
            }
            solution[i] = local_sum;
        } else {
            // For small vectors, use sequential summation
            solution[i] = std::accumulate(data[i].begin(), data[i].end(), 0);
        }
    }
}
 
// Parallelize across vectors using static scheduling
// Best suited for many small vectors of roughly equal size
void vector_sum_omp_static(const InputVectors& data, OutputVector& solution, size_t min_vector_size) {
    #pragma omp parallel for schedule(static)
    for (size_t i = 0; i < data.size(); ++i) {
        solution[i] = std::accumulate(data[i].begin(), data[i].end(), 0);
    }
}
 
void vector_sum_omp_dynamic(const InputVectors& data, OutputVector& solution, size_t min_vector_size) {
    #pragma omp parallel for schedule(dynamic)
    for (size_t i = 0; i < data.size(); ++i) {
        solution[i] = std::accumulate(data[i].begin(), data[i].end(), 0);
    }
}
 
// Pre-shuffle data to improve load balancing
// Uses static scheduling after shuffling
void vector_sum_omp_shuffle(const InputVectors& data, OutputVector& solution, size_t min_vector_size) {
    std::vector<size_t> indices(data.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), std::mt19937(std::random_device()()));
 
    #pragma omp parallel for schedule(static)
    for (size_t idx = 0; idx < indices.size(); ++idx) {
        size_t i = indices[idx];
        solution[i] = std::accumulate(data[i].begin(), data[i].end(), 0);
    }
}
