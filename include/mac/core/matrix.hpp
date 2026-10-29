#pragma once

#include <cstddef>
#include <vector>

namespace mac {

// Dense, row-major matrix. Element offset: i * cols() + j.
class Matrix {
public:
    Matrix(std::size_t rows, std::size_t cols);

    [[nodiscard]] std::size_t rows() const noexcept;
    [[nodiscard]] std::size_t cols() const noexcept;
    [[nodiscard]] const std::vector<double>& data() const noexcept;

    // Unchecked, for validated computational kernels.
    double& operator()(std::size_t i, std::size_t j) noexcept;
    const double& operator()(std::size_t i, std::size_t j) const noexcept;

    // Checked, for development and boundary-sensitive access.
    double& at(std::size_t i, std::size_t j);
    const double& at(std::size_t i, std::size_t j) const;

private:
    std::size_t rows_;
    std::size_t cols_;
    std::vector<double> data_;
};

} // namespace mac
