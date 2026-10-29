#pragma once
#include "mac/core/matrix.hpp"

namespace mac {

// Course Lab 01: implement these three methods yourself.
// Precondition: A.cols() == B.rows(). Return A.rows() x B.cols().
Matrix matmul_ijk(const Matrix& a, const Matrix& b);
Matrix matmul_ikj(const Matrix& a, const Matrix& b);
Matrix matmul_kij(const Matrix& a, const Matrix& b);

} // namespace mac
