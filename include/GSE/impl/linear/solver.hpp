// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "../core/traits.hpp"
#include "../core/types.hpp"

#include "./method/full_pivot_lu.hpp"
#include "./method/householder_qr.hpp"
#include "./method/llt.hpp"
#include "./method/partial_pivot_lu.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Numerical solution of linear systems.
//
// All methods simply wrap Eigen routines into a GSE-style API,
// so we can have the same kind of algorithm composability as with other solvers.
//
// All underlying methods are direct, iterative solvers in Eigen are mostly useful for sparse problems
// and they introduce some architectural "ugliness" due having a different signature, which makes it
// difficult to propagate precision / max iteration parameters through other generic methods.
//
// Method overview can be found here:
// https://eigen.tuxfamily.org/dox/group__TutorialLinearAlgebra.html
//
// Decomposition benchmarks can be found here:
// https://eigen.tuxfamily.org/dox/group__DenseDecompositionBenchmark.html

// ____________________ IMPLEMENTATION ____________________

// ================
// --- Defaults ---
// ================

namespace gse::impl::linear::defaults {

using linear_method = method::PartialPivotLU;

}

// ==============
// --- SFINAE ---
// ==============

namespace gse::impl::linear {

template <class T, Extent N, class Method>
using require_valid_method = require_invocable_r<Vector<T, N>, Method, Matrix<T, N, N>, Vector<T, N>>;

} // namespace gse::impl::linear

// ==============
// --- Solver ---
// ==============

namespace gse::impl::linear {

// Solves linear system:
//    A x = b,   A - NxN, b - Nx1.
//
// here:
//    'A'      - system matrix,
//    'b'      - system RHS,
//    'method' - direct linear method.
//
template <class T, Extent N, class Method = defaults::linear_method, require_valid_method<T, N, Method> = true>
Vector<T, N> solve(const Matrix<T, N, N>& A, const Vector<T, N>& b, Method&& method = Method{}) {
    return method(A, b);
}

} // namespace gse::impl::linear
