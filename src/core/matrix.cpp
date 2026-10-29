#include "mac/core/matrix.hpp"

#include <limits>
#include <stdexcept>

namespace mac {

Matrix::Matrix(std::size_t rows, std::size_t cols)
    : rows_(rows), cols_(cols), data_() {
    if (cols != 0 && rows > std::numeric_limits<std::size_t>::max() / cols) {
        throw std::length_error("Matrix dimensions overflow size_t");
    }
    data_.resize(rows * cols, 0.0);
}

std::size_t Matrix::rows() const noexcept { return rows_; }
std::size_t Matrix::cols() const noexcept { return cols_; }
const std::vector<double>& Matrix::data() const noexcept { return data_; }

double& Matrix::operator()(std::size_t i, std::size_t j) noexcept {
    return data_[i * cols_ + j];
}
const double& Matrix::operator()(std::size_t i, std::size_t j) const noexcept {
    return data_[i * cols_ + j];
}

double& Matrix::at(std::size_t i, std::size_t j) {
    if (i >= rows_ || j >= cols_) throw std::out_of_range("Matrix index out of range");
    return data_[i * cols_ + j];
}
const double& Matrix::at(std::size_t i, std::size_t j) const {
    if (i >= rows_ || j >= cols_) throw std::out_of_range("Matrix index out of range");
    return data_[i * cols_ + j];
}

} // namespace mac
