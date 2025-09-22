// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "GSE/jacobian/methods/central_difference.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Numerical jacobian computation.

// ____________________ IMPLEMENTATION ____________________

namespace gse::jacobian {

// ================
// --- Defaults ---
// ================

namespace defaults {

// [ LSP documentation ]
//
// Default method for computing a jacobian. Templated on:
//    `T` - scalar type.
//
template <scalar T>
using method = methods::CentralDifference<T>;

}

// ==============
// --- Solver ---
// ==============

// [ LSP documentation ]
//
// Computes jacobian of a vector function:
//    `f: R^N -> R^N`
//
// where:
//    `f`      - vector-function,
//    `x`      - evaluation point,
//    `method` - numerical differentiation method.
//
template <scalar T, Extent N, vector_function<T, N> Func, method_function<T, N> Method = defaults::method<T>>
Matrix<T, N, N> solve(Func&& f, const Vector<T, N>& x, Method&& method = Method{}) {
    return method(f, x);
}

} // namespace gse::linear
