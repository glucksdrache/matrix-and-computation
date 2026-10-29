# MATRIX AND COMPUTATION

A long-running, textbook-grounded course project connecting matrix analysis, numerical linear algebra, C++ implementation, and reproducible experiments.

## Session 01

**Read:** Golub & Van Loan, *Matrix Computations* (4th ed.), §1.1.12–1.1.15; Trefethen & Bau, *Numerical Linear Algebra*, Lecture 1.

**Build** (CMake >= 3.20, C++20, Ninja):

```bash
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

**Student task:** implement `matmul_ijk`, `matmul_ikj`, `matmul_kij` in `src/algorithms/matmul.cpp` and then add correctness checks in `tests/test_matmul.cpp` and enable the test target in CMake. The methods currently throw `logic_error` intentionally.

**Commit workflow** (once the local checkout has its Git remote configured):

```bash
git status
git add .
git commit -m "chore: initialize matrix-and-computation project"
git push -u origin main
```

`build/` is ignored; build outputs are not committed.

## Structure
- `include/mac`: public C++ interfaces
- `src`: implementations
- `tests`: CTest-based correctness tests
- `benchmarks`: timing experiments (future lab)
- `docs`: theory, experiments, course state, handoffs
- `notebooks`: Python validation and visualization
