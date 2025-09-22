// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "GSE/nonlinear/methods/newton.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Numerical solution of nonlinear systems.
//
// Solver wraps iterative root finding method in a loop with stop conditions.

// ____________________ IMPLEMENTATION ____________________

namespace gse::nonlinear {

// ================
// --- Defaults ---
// ================

namespace defaults {

// [ LSP documentation ]
//
// Default method for solving nonlinear systems.
//
template <scalar T, Extent N>
using method = methods::Newton<T, N>;

}

// ==============
// --- Solver ---
// ==============

// [ LSP documentation ]
//
// Solves nonlinear system iteratively:
//    `f(x) = 0,   f: R^N -> R^N`
//
// where:
//    `f`      - system,
//    `x0`     - initial guess,
//    `method` - root finding method.
//
template <scalar T, Extent N, vector_function<T, N> Func, method_function<T, N> Method = defaults::method<T, N>>
Vector<T, N> solve(Func&& f, Vector<T, N> x0, Method&& method = Method{}) {
    Vector<T, N> x = x0;
    
    for (Uint iteration = 0; iteration < method.max_iterations; ++iteration) {
        x = method(f, std::move(x));
        if ((x - x0).norm() < method.precision) break;
        x0 = x;
    }
    
    return x;
}

// TODO: Callback overload

} // namespace gse::linear
