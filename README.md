# 🎮 Vector Summation Strategies in C++ – A Game Performance Playground

This project explores **parallelization strategies for summing vectors**, a common operation in **real-time systems and game engines**.  
Whether you’re dealing with physics vectors, AI steering data, or animation transforms — **how** you sum matters.

This repo benchmarks multiple threading approaches to summing large batches of vectors with different distribution patterns.

---

## 🕹️ Why It Matters in Games

Games constantly process massive amounts of spatial and numeric data:
- **Movement systems**: summing forces or directions
- **AI**: combining sensory inputs or weights
- **Animation**: blending vectors across rigs or timelines

If your logic processes hundreds of thousands of vectors **per frame**, summing them the wrong way can blow your frame budget.

This project started as a deep dive into:
- 🧵 Threading models (`std::thread`, OpenMP)
- ⚖️ Load balancing
- 🧠 Real-time performance insights for uneven data

---

## 🚀 Methods Compared

Each method was implemented and benchmarked on datasets that simulate **real-world variances** you'd find in gameplay systems:

| Method | Strategy |
|--------|----------|
| 🐢 `sequential_sum` | Baseline, single-threaded |
| ⚡ `sum_per_vector_parallel` | One thread per vector (`std::thread`) |
| 🎲 `sum_with_shuffling` | Shuffled data for better load balancing |
| 🔄 `omp_dynamic_schedule` | OpenMP with dynamic scheduling |
| 🧱 `omp_static_schedule` | OpenMP with fixed-size chunks |

---

## 🧪 Dataset Types

| Dataset | Simulates... |
|---------|--------------|
| Few very large vectors | Heavy physics objects |
| Mixed sizes | NPCs with dynamic behaviors |
| Millions of tiny vectors | Particle systems or grass blades |
| “Bad for parallelism” | Real-time edge cases (e.g. frame spikes) |

Each method was stress-tested against these inputs. Results are displayed in a clear ASCII table (see below).

---

## 📊 Sample Output

```text
|                        | Sequential | Vector-level parallelism | Shuffling | Dynamic scheduling | Static scheduling |
|------------------------|------------|---------------------------|-----------|---------------------|--------------------|
| Few very long vectors  |    3.11 s  |          1.44 s          |   1.50 s  |        1.12 s       |       1.30 s       |
| High variance in size  |    2.01 s  |          0.89 s          |   1.01 s  |        0.73 s       |       0.76 s       |
| Many short vectors     |    0.89 s  |          0.42 s          |   0.49 s  |        0.31 s       |       0.33 s       |
| Bad for parallelism    |    0.00 s  |          0.01 s          |   0.01 s  |        0.01 s       |       0.01 s       |
```

---

## 📁 File Overview
```bash
├── vector_sum.cpp      # ⭐ All summing logic lives here
├── vector_sum.h        # Interface for summing functions
├── main.cpp            # Benchmark runner
├── executor.h          # Timing and execution wrappers
├── data_generator.h    # Builds different input datasets
├── TextTable.h         # Pretty-print results to ASCII
└── README.md           # This file
```

---

## ⚙️ How to Run
Requires a C++17 compiler with OpenMP support (g++, clang++)
```bash
g++ -std=c++17 -fopenmp main.cpp -o benchmark
./benchmark
```

---

## 🧠 Key Learnings (Game-Oriented)

- 🧩 Not all parallelization strategies scale equally
- ⚖️ Load distribution matters more than thread count
- 🎯 Some operations need work stealing or dynamic chunking to stay real-time
- 🧼 Clean threading code is testable and benchmarkable — not guesswork

---

## 🛠️ Potential Game-Engine Extensions

- Replace synthetic vectors with physics simulation data
- Integrate with Unity ECS or Unreal's task graph
- Port to SIMD-accelerated instructions for per-frame summing
- Visualize benchmark profiles with Perfetto or Tracy

---

## 🧷 License

MIT — use, break, fork, or plug into your game engine.

## 👤 Author
🦾 Crafted by Aleksandra Kenig (aka [yourpunk](https://github.com/yourpunk))- game developer exploring the guts of performance programming.

### 💌 Want to nerd out about thread pools, physics engines or how bad memory layout can ruin your framerate? Ping me.
