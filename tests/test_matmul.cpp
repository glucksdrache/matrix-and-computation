// Lab 01 follow-up: enable this test in CMake after implementing all three kernels.
// Required cases: non-square A(2x3)*B(3x4), zero matrices, dimension mismatch,
// and cross-comparison among ijk, ikj, kij (with floating-point tolerance).
#include "mac/algorithms/matmul.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>

namespace {

constexpr double eps = 1e-12;

void assert_close(const mac::Matrix& lhs, const mac::Matrix& rhs) {
    assert(lhs.rows() == rhs.rows() && lhs.cols() == rhs.cols());
    for (std::size_t i = 0; i < lhs.rows(); ++i) {
        for (std::size_t j = 0; j < lhs.cols(); ++j) {
            assert(std::abs(lhs(i, j) - rhs(i, j)) <= eps);
        }
    }
}

void print_matrix(const mac::Matrix& matrix) {
    for (std::size_t i = 0; i < matrix.rows(); ++i) {
        std::cout << "  [ ";
        for (std::size_t j = 0; j < matrix.cols(); ++j) {
            std::cout << matrix(i, j) << ' ';
        }
        std::cout << "]\n";
    }
}

bool rejects_mismatch(mac::Matrix (*multiply)(const mac::Matrix&, const mac::Matrix&)) {
    try {
        const mac::Matrix a(2, 3);
        const mac::Matrix b(2, 2);
        (void)multiply(a, b);
    } catch (const std::invalid_argument&) {
        return true;
    }
    return false;
}

} // namespace

int main() {
    mac::Matrix a(2, 3);
    mac::Matrix b(3, 4);

    a(0, 0) = 1; a(0, 1) = 2; a(0, 2) = 3;
    a(1, 0) = 4; a(1, 1) = 5; a(1, 2) = 6;

    b(0, 0) = 1; b(0, 1) = 0; b(0, 2) = 2; b(0, 3) = 1;
    b(1, 0) = 0; b(1, 1) = 1; b(1, 2) = 1; b(1, 3) = 2;
    b(2, 0) = 2; b(2, 1) = 0; b(2, 2) = 1; b(2, 3) = 0;

    const auto c1 = mac::matmul_ijk(a, b);
    const auto c2 = mac::matmul_ikj(a, b);
    const auto c3 = mac::matmul_kij(a, b);

    std::cout << "[1] Non-square multiplication: (2x3) * (3x4) = (2x4)\n";
    print_matrix(c1);

    assert(c1.rows() == 2);
    assert(c1.cols() == 4);

    assert(c1(0, 0) == 7);
    assert(c1(0, 1) == 2);
    assert(c1(0, 2) == 7);
    assert(c1(0, 3) == 5);

    assert(c1(1, 0) == 16);
    assert(c1(1, 1) == 5);
    assert(c1(1, 2) == 19);
    assert(c1(1, 3) == 14);
    std::cout << "    PASS: dimensions and values\n";

    assert_close(c1, c2);
    assert_close(c1, c3);
    std::cout << "[2] Cross-comparison (ijk, ikj, kij; eps=" << eps << ")\n"
              << "    PASS: all three kernels agree\n";

    const mac::Matrix zero_a(2, 3);
    const mac::Matrix zero_b(3, 4);
    const mac::Matrix expected_zero(2, 4);
    assert_close(mac::matmul_ijk(zero_a, b), expected_zero);
    assert_close(mac::matmul_ikj(a, zero_b), expected_zero);
    assert_close(mac::matmul_kij(zero_a, zero_b), expected_zero);
    std::cout << "[3] Zero-matrix multiplication\n"
              << "    PASS: zero*A, A*zero, and zero*zero produce zero\n";

    assert(rejects_mismatch(mac::matmul_ijk));
    assert(rejects_mismatch(mac::matmul_ikj));
    assert(rejects_mismatch(mac::matmul_kij));
    std::cout << "[4] Dimension mismatch: (2x3) * (2x2)\n"
              << "    PASS: all three kernels throw std::invalid_argument\n"
              << "\nAll matmul checks passed.\n";
}
