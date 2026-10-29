#include "mac/algorithms/matmul.hpp"
#include <cstddef>
#include <stdexcept>

namespace mac {

Matrix matmul_ijk(const Matrix& a, const Matrix& b) {
    if (a.cols() != b.rows()) {
        throw std::invalid_argument("Incomplete dimensions");
    }

    const std::size_t m = a.rows();
    const std::size_t n = a.cols();
    const std::size_t p = b.cols();

    Matrix c(m, p);

    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < p; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < n; k++) {
                sum += a(i, k) * b(k, j);
            }
            c(i, j) = sum;
        }
    }         

    return c;
}

Matrix matmul_ikj(const Matrix& a, const Matrix& b) {
    // TODO (student): check dimensions; use i-k-j loop nesting.
    (void)a; (void)b;
    throw std::logic_error("TODO: implement matmul_ikj");
}

Matrix matmul_kij(const Matrix& a, const Matrix& b) {
    // TODO (student): check dimensions; use k-i-j loop nesting.
    (void)a; (void)b;
    throw std::logic_error("TODO: implement matmul_kij");
}

} // namespace mac
