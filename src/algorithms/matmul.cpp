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

    for (std::size_t i = 0; i < m; i++) {
        for (std::size_t j = 0; j < p; j++) {
            double sum = 0.0;
            for (std::size_t k = 0; k < n; k++) {
                sum += a(i, k) * b(k, j);
            }
            c(i, j) = sum;
        }
    }         

    return c;
}

Matrix matmul_ikj(const Matrix& a, const Matrix& b) {
    if (a.cols() != b.rows()) {
        throw std::invalid_argument("Incomplete dimensions");
    }

    const std::size_t m = a.rows();
    const std::size_t n = a.cols();
    const std::size_t p = b.cols();

    Matrix c(m, p);

    for (std::size_t i = 0; i < m; i++) {
        for (std::size_t k = 0; k < n; k++) {
            for (std::size_t j = 0; j < p; j++) {
                //这里c(i, j)不能复用，需要访问c(i, :)
                // 不过好处是c(i, :)和b(k, :)都比较小可以放进缓存，而且在内存中是连续访问
                c(i, j) += a(i, k) * b(k, j);
            }
        }
    }

    return c;
}

Matrix matmul_kij(const Matrix& a, const Matrix& b) {
    if (a.cols() != b.rows()) {
        throw std::invalid_argument("Incomplete dimensions");
    }

    std::size_t m = a.rows();
    std::size_t n = a.cols();
    std::size_t p = b.cols();

    Matrix c(m, p);

    for (std::size_t k = 0; k < n; k++) {
        for (std::size_t i = 0; i < m; i++) {
            for (std::size_t j = 0; j < p; j++) {
                //外积形式，c太大时放不进缓存，需反复读取
                c(i, j) += a(i, k) * b(k, j);
            }
        }
    }

    return c;
}

} // namespace mac
