#include "mac/algorithms/matmul.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
using Clock = std::chrono::steady_clock;
using Kernel = mac::Matrix (*)(const mac::Matrix&, const mac::Matrix&);

mac::Matrix input(std::size_t rows, std::size_t cols, unsigned seed) {
    mac::Matrix a(rows, cols);
    for (std::size_t i = 0; i < rows; ++i)
        for (std::size_t j = 0; j < cols; ++j)
            a(i, j) = (static_cast<double>((i * 37 + j * 17 + seed) % 101) - 50.0) / 101.0;
    return a;
}

double checksum(const mac::Matrix& a) {
    double s = 0.0;
    for (std::size_t i = 0; i < a.rows(); ++i)
        for (std::size_t j = 0; j < a.cols(); ++j)
            s += a(i, j);
    return s;
}

bool close(const mac::Matrix& a, const mac::Matrix& b) {
    if (a.rows() != b.rows() || a.cols() != b.cols()) return false;
    for (std::size_t i = 0; i < a.rows(); ++i)
        for (std::size_t j = 0; j < a.cols(); ++j) {
            const double x = a(i, j), y = b(i, j);
            if (std::abs(x - y) > 1e-10 * (1.0 + std::max(std::abs(x), std::abs(y))))
                return false;
        }
    return true;
}

struct Result { const char* name; double median_ms; double gflops; double sum; };

Result measure(const char* name, Kernel kernel, const mac::Matrix& a,
               const mac::Matrix& b, const mac::Matrix& reference,
               int repetitions) {
    // Warm-up, correctness validation, and protection from unused-result elimination.
    const auto warm = kernel(a, b);
    if (!close(warm, reference)) throw std::runtime_error(std::string(name) + " failed validation");
    std::vector<double> times;
    times.reserve(static_cast<std::size_t>(repetitions));
    double last_sum = 0.0;
    for (int r = 0; r < repetitions; ++r) {
        const auto t0 = Clock::now();
        auto c = kernel(a, b);
        const auto t1 = Clock::now();
        last_sum = checksum(c);
        times.push_back(std::chrono::duration<double, std::milli>(t1 - t0).count());
    }
    std::sort(times.begin(), times.end());
    const double median = times[times.size() / 2];
    const double m = static_cast<double>(a.rows()),
                 n = static_cast<double>(a.cols()),
                 p = static_cast<double>(b.cols());
    const double flops = m * p * (2.0 * n - 1.0);
    return {name, median, flops / (median * 1e6), last_sum};
}
}

int main(int argc, char** argv) {
    try {
        const int repetitions = (argc > 1) ? std::stoi(argv[1]) : 5;
        if (repetitions < 1 || repetitions > 99) throw std::invalid_argument("repetitions must be 1..99");
        std::cout << "n,kernel,median_ms,gflops,checksum\n";
        for (const std::size_t n : {64u, 128u, 256u, 512u}) {
            auto a = input(n, n, 3);
            auto b = input(n, n, 11);
            const auto reference = mac::matmul_ijk(a, b);
            // Rotate execution order to reduce systematic warm-cache advantage.
            const struct { const char* name; Kernel fn; } kernels[] = {
                {"ijk", mac::matmul_ijk}, {"ikj", mac::matmul_ikj}, {"kij", mac::matmul_kij}
            };
            for (int offset = 0; offset < 3; ++offset) {
                const auto& k = kernels[(offset + static_cast<int>((n / 64) % 3)) % 3];
                const auto r = measure(k.name, k.fn, a, b, reference, repetitions);
                std::cout << n << ',' << r.name << ',' << std::fixed << std::setprecision(4)
                          << r.median_ms << ',' << std::setprecision(3) << r.gflops
                          << ',' << std::setprecision(8) << r.sum << '\n';
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "benchmark error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
}
