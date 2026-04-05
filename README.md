# Smallest Enclosing Circle (MEC) Project

This project explores multiple algorithms and approaches for computing the **Smallest Enclosing Circle (MEC)**, including deterministic, randomized (CGAL-based), and sampling-based techniques. It also includes benchmarking utilities and an interactive simulation.

---

## ⚙️ Algorithms Overview

### 1. `mec.cpp`
- Deterministic **O(n³) incremental algorithm**
- Takes input size `n` and computes MEC
- No external dependencies

---

### 2. `mec_cgal.cpp`
- Uses **CGAL's randomized MEC algorithm**
- Efficient and practical implementation
- Requires CGAL dependencies (`gmp`, `mpfr`)

---

### 3. `benchmark.cpp`
- Benchmarks CGAL-based MEC
- Runs experiments for input sizes from **100 to 10⁶**
- Executes **10,000 iterations per input size**
- Reports:
  - Average runtime
  - Number of iterations deviating from average

---

### 4. `random_sampling.cpp`
- Evaluates sampling-based heuristics
- For input sizes from **100 to 10⁶**
- Samples `k` points where:
  - `k = log(n)`
  - `k = n^(1/3)`
  - `k = sqrt(n)`
  - `k = n/2`
- Reports number of **violations** for each sampling strategy

---

## 🔧 Compilation Instructions

### Compile Deterministic Version

```bash
g++ -O3 mec.cpp -o mec
```

---

### Compile CGAL-based Programs

Make sure CGAL dependencies are installed (`gmp`, `mpfr`).

```bash
g++ -O3 mec_cgal.cpp -lgmp -lmpfr -o mec_cgal
g++ -O3 benchmark.cpp -lgmp -lmpfr -o benchmark
g++ -O3 random_sampling.cpp -lgmp -lmpfr -o random_sampling
```

---

## ▶️ Running the Programs

### Deterministic MEC
```bash
./mec
```

### CGAL MEC
```bash
./mec_cgal
```

### Benchmark
```bash
./benchmark
```

### Random Sampling Analysis
```bash
./random_sampling
```

---

## 🌐 Simulation

The `simulation/` folder contains an interactive visualization of all implemented algorithms.

### Features:
- Step-by-step execution
- Visual comparison of algorithms
- Demonstrates correctness and performance differences

### Live Demo:
👉 https://smallest-enclosing-circle.onrender.com/

---

## 🐳 Simulation Setup (Optional)

Inside `simulation/`:

```bash
docker build -t mec-sim .
docker run -p 8000:8000 mec-sim
```

Or run manually:

```bash
pip install -r requirements.txt
python main.py
```

---

## 📌 Notes

- CGAL-based implementations rely on **randomization**
- Deterministic implementation is primarily for **theoretical comparison**
- Sampling approach explores **trade-offs between accuracy and performance**

---

## 🚀 Summary

| File                | Approach              | Complexity     | Notes                          |
|---------------------|----------------------|----------------|--------------------------------|
| `mec.cpp`           | Deterministic        | O(n³)          | Baseline implementation        |
| `mec_cgal.cpp`      | CGAL Randomized      | Expected O(n)  | Practical and efficient        |
| `benchmark.cpp`     | Performance Analysis | —              | 10k iterations per input size  |
| `random_sampling.cpp` | Sampling Heuristic | Varies         | Tests multiple sampling sizes  |

---

## 👨‍💻 Author

Developed as part of coursework/project for computational geometry and performance analysis.
