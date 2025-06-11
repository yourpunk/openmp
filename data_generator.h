#pragma once

#include "vector_sum.h"

#include <random>
#include <stdexcept>

namespace data_generator {

    // Uniform random number generator
    // Parameters:
    //      data     - input data vectors
    //      solution - sum of each vector will be stored here
    inline void generate_data(OutputVector& solution, InputVectors& data) {
        if (data.size() != solution.size()) {
            throw std::invalid_argument("Solution vector and number of data vectors must match.");
        }

        std::mt19937 gen{0}; // PRNG with fixed seed
        std::uniform_int_distribution<int8_t> dist{
            std::numeric_limits<int8_t>::min(),
            std::numeric_limits<int8_t>::max()
        };

        // Generate values and compute sums
        for (size_t i = 0; i < data.size(); i++) {
            int64_t sum = 0;
            for (auto& j : data[i]) {
                int8_t number = dist(gen);
                j = number;
                sum += number;
            }
            solution[i] = sum;
        }
    }

    // Generate random vector sizes using normal distribution
    // Parameters:
    //      begin, end - iterator range to fill with generated sizes
    //      mean       - mean of the distribution
    //      sigma      - standard deviation of the distribution
    template<typename Iter>
    inline void generate_vector_sizes(Iter begin, Iter end, float mean, float sigma) {
        std::mt19937 gen(0); // PRNG with fixed seed
        std::normal_distribution<float> d(mean, sigma);

        for (auto it = begin; it != end; ++it) {
            *it = static_cast<size_t>(std::max(std::round(d(gen)), 1.0f));
        }
    }
}
