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

using nonlinear_method = method::Newton<>;

template <class T>
constexpr T precision = is_float_v<T> ? 1e-6 : 1e-10;

template <class T>
constexpr T max_iterations = is_float_v<T> ? 80 : 100;

} // namespace gse::impl::nonlinear::defaults

// ==============
// --- SFINAE ---
// ==============

namespace gse::impl::nonlinear {

template <class T, Extent N, class Func>
using require_valid_func = require_invocable_r<Vector<T, N>, Func, Vector<T, N>>;

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
//    'precision'      - target iteration difference,
//    'max_iterations' - max number of nonlinear iterations,
//    'method'         - method of nonlinear iteration.
//
template <class T, Extent N, class Func, class Method = method::Newton<>, require_valid_func<T, N, Func> = true,
          require_valid_method<T, N, Func, Method> = true>
Vector<T, N> solve(Func&&       f,                                            //
                   Vector<T, N> x0,                                           //
                   T            precision      = defaults::precision<T>,      //
                   Uint         max_iterations = defaults::max_iterations<T>, //
                   Method&&     method         = Method{}                     //
) {                                                                           //

    Vector<T, N> x = x0;

    for (Uint iteration = 0; iteration < max_iterations; ++iteration) {
        x = method(f, std::move(x));
        if ((x - x0).norm() < precision) break;
        x0 = x;
    }

    return x;
}

} // namespace gse::impl::nonlinear
