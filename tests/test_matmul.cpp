// Lab 01 follow-up: enable this test in CMake after implementing all three kernels.
// Required cases: non-square A(2x3)*B(3x4), zero matrices, dimension mismatch,
// and cross-comparison among ijk, ikj, kij (with floating-point tolerance).
#include "mac/algorithms/matmul.hpp"

#include <cassert>

int main() {
    mac::Matrix a(2, 3);
    mac::Matrix b(3, 4);

    a(0, 0) = 1; a(0, 1) = 2; a(0, 2) = 3;
    a(1, 0) = 4; a(1, 1) = 5; a(1, 2) = 6;

    b(0, 0) = 1; b(0, 1) = 0; b(0, 2) = 2; b(0, 3) = 1;
    b(1, 0) = 0; b(1, 1) = 1; b(1, 2) = 1; b(1, 3) = 2;
    b(2, 0) = 2; b(2, 1) = 0; b(2, 2) = 1; b(2, 3) = 0;

    const mac::Matrix c = mac::matmul_ijk(a, b);

    assert(c.rows() == 2);
    assert(c.cols() == 4);

    assert(c(0, 0) == 7);
    assert(c(0, 1) == 2);
    assert(c(0, 2) == 7);
    assert(c(0, 3) == 5);

    assert(c(1, 0) == 16);
    assert(c(1, 1) == 5);
    assert(c(1, 2) == 19);
    assert(c(1, 3) == 14);
}