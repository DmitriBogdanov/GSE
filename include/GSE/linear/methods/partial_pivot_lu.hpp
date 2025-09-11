// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "GSE/linear/method.hpp"

// ____________________ DEVELOPER DOCS ____________________

// None

// ____________________ IMPLEMENTATION ____________________

namespace gse::linear::methods {

// [ LSP documentation ]
//
// Direct method for solving systems of linear equations.
// 
// A good default for most problems.
//
template <scalar T, Extent N>
struct PartialPivotLU {

    Vector<T, N> operator()(const Matrix<T, N, N>& A, const Vector<T, N>& b) {
        return A.partialPivLu().solve(b);
    }
};

static_assert(method_function<PartialPivotLU<double, dynamic>, double, dynamic>);

} // namespace gse::impl::linear::method