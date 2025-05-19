// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "../../core/types.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Numerical direct SLAE-solving method. Uses full pivot LU decomposition from Eigen.

// ____________________ IMPLEMENTATION ____________________

namespace gse::impl::linear::method {

struct FullPivotLU {
    template <class T, Extent N>
    Vector<T, N> operator()(const Matrix<T, N, N>& A, const Vector<T, N>& b) {
        return A.fullPivLu().solve(b);
    }
};

} // namespace gse::impl::jacobian::method
