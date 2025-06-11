# 🧮 Vector Sum Benchmark Playground 

Welcome to the chaos lab. This is a benchmarking sandbox for testing different vector summing strategies — from boring sequential to OpenMP wizardry.  
The goal? To see how different implementations perform across datasets with different vector characteristics.

❗ The `vector_sum.cpp` file was written by me, from scratch. I built and tested multiple methods for summing vectors, with and without multithreading.

---

## 📦 What's inside

This project compares 5 different methods for summing vectors:

- 🐢 `sequential_sum` – no threads, no surprises.
- ⚡ `sum_per_vector_parallel` – parallelize across vectors (std::thread).
- 🎲 `sum_with_shuffling` – reorder vectors before parallelizing.
- 🔄 `omp_dynamic_schedule` – OpenMP with dynamic scheduling.
- 🧱 `omp_static_schedule` – OpenMP with static scheduling.

Each method is benchmarked using different datasets:
- Few very long vectors
- Mixed (some big, some tiny)
- Millions of short vectors
- Useless tiny input that hates parallelism

Results are printed in a clean **ASCII** table.

## 🧠 Why I made this

I wanted to dig deep into **parallelization strategies** and see how real-world things behave — not just read a blog post.  
Instead of just using OpenMP blindly, I wanted to **write it**, **run it**, **break it**, and **compare it**.


## 🧵 My part of the project

The file `vector_sum.cpp` (and `vector_sum.h`) is where I wrote the actual logic for the five summing methods.  
It includes thread-safe execution, chunking logic, OpenMP directives, and careful use of `std::thread`.

If you're here to look at **how vector summing can be written and compared in different threading models**, that's the file to read.


## 🧪 How to run

> Make sure you have a C++ compiler with OpenMP support (like `g++` or `clang++`).

```bash
g++ -std=c++17 -fopenmp main.cpp -o benchmark
./benchmark
```

## 📊 Sample output

|                        | Sequential | Vector-level parallelism | Shuffling | Dynamic scheduling | Static scheduling |
|------------------------|------------|---------------------------|-----------|---------------------|--------------------|
| Few very long vectors  |    3.11 s  |          1.44 s          |   1.50 s  |        1.12 s       |       1.30 s       |
| High variance in size  |    2.01 s  |          0.89 s          |   1.01 s  |        0.73 s       |       0.76 s       |
| Many short vectors     |    0.89 s  |          0.42 s          |   0.49 s  |        0.31 s       |       0.33 s       |
| Bad for parallelism    |    0.00 s  |          0.01 s          |   0.01 s  |        0.01 s       |       0.01 s       |

## 👀 Files overview
| File |	Description|
|------|-------------|
| vector_sum.cpp |	⭐ My code. All summing methods live here.|
| main.cpp	| Runs the tests, builds table output.|
| executor.h	| Calls the summing methods, wraps them with timers.\
| data_generator.h	| Builds different datasets.|
| TextTable.h	| Simple ASCII table generator.|

## 🧩 License
**MIT**. Use it, break it, remix it. Just don't gatekeep knowledge. Credit appreciated, but not required.

## 👤 Author
🦾 Crafted by Aleksandra Kenig (aka [yourpunk](https://github.com/yourpunk)).
💌 Wanna collab or throw some feedback? You know where to find me.
