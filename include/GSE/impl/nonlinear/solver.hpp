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

#include "./method/newton.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Numerical solution of nonlinear systems.
//
// Solver wraps iterative root finding method in a loop with stop conditions.

// TODO: Add callback overload

// ____________________ IMPLEMENTATION ____________________

// ================
// --- Defaults ---
// ================

namespace gse::impl::nonlinear::defaults {

template <class T>
using nonlinear_method = method::Newton<T>;

} // namespace gse::impl::nonlinear::defaults

// ==============
// --- SFINAE ---
// ==============

namespace gse::impl::nonlinear {

template <class T, Extent N, class Func, class Method>
using require_valid_method = require_invocable_r<Vector<T, N>, Method, Func, Vector<T, N>>;

} // namespace gse::impl::nonlinear

// ==============
// --- Solver ---
// ==============

namespace gse::impl::nonlinear {

// Solve nonlinear system of equations iteratively:
//    { f(x) = 0,   f: R^N -> R^N
//
// here:
//    'f'              - system,
//    'x0'             - initial guess,
//    'method'         - method of nonlinear iteration.
//
template <class T, Extent N, class Func, class Method = method::Newton<T>, require_vector_function<T, N, Func> = true,
          require_valid_method<T, N, Func, Method> = true>
Vector<T, N> solve(Func&&       f,                //
                   Vector<T, N> x0,               //
                   Method&&     method = Method{} //
) {                                               //
    // Note: 'base::Common<T>' guarantees presense of 'precision' and 'max_iterations'

    Vector<T, N> x = x0;

    for (Uint iteration = 0; iteration < method.max_iterations; ++iteration) {
        x = method(f, std::move(x));
        if ((x - x0).norm() < method.precision) break;
        x0 = x;
    }

    return x;
}

} // namespace gse::impl::nonlinear
