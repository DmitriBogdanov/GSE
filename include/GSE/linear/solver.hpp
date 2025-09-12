// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "GSE/linear/methods/partial_pivot_lu.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Numerical solution of square linear systems.
//
// All methods simply wrap Eigen routines into a GSE-style API,
// so we can have the same kind of algorithm composability as with other solvers.
//
// All underlying methods are direct, iterative solvers in Eigen are mostly useful for sparse problems
// and they introduce some architectural "ugliness" due having a different signature, which makes it
// difficult to propagate precision / max iteration parameters through other generic methods.
//
// Overview of various methods can be found here:
// https://eigen.tuxfamily.org/dox/group__TutorialLinearAlgebra.html
//
// Decomposition benchmarks can be found here:
// https://eigen.tuxfamily.org/dox/group__DenseDecompositionBenchmark.html

// ____________________ IMPLEMENTATION ____________________

namespace gse::linear {

// ================
// --- Defaults ---
// ================

namespace defaults {

// [ LSP documentation ]
//
// Default method for solving square linear systems. Templated on:
//    `T` - scalar type,
//    `N` - system extent (static or dynamic).
//
template <scalar T, Extent N>
using method = methods::PartialPivotLU<T, N>;

}

// ==============
// --- Solver ---
// ==============

// [ LSP documentation ]
//
// Solves linear system:
//    `A x = b,   A - NxN, b - Nx1.`
//
// where:
//    `A`      - system matrix,
//    `b`      - system RHS,
//    `method` - linear method.
//
template <scalar T, Extent N, method_function<T, N> Method = defaults::method<T, N>>
Vector<T, N> solve(const Matrix<T, N, N>& A, const Vector<T, N>& b, Method&& method = Method{}) {
    return method(A, b);
}

} // namespace gse::linear
