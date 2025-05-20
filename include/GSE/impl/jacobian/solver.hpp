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

#include "./method/central_difference.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Numerical jacobian evaluation.
//
// Solver doesn't do much beyond wrapping the method.

// ____________________ IMPLEMENTATION ____________________

// ================
// --- Defaults ---
// ================

namespace gse::impl::jacobian::defaults {

template <class T>
using jacobian_method = method::CentralDifference<T>;

} // namespace gse::impl::jacobian::defaults

// ==============
// --- SFINAE ---
// ==============

namespace gse::impl::jacobian {

template <class T, Extent N, class Func, class Method>
using require_valid_method = require_invocable_r<Matrix<T, N, N>, Method, Func, Vector<T, N>>;

} // namespace gse::impl::jacobian

// ==============
// --- Solver ---
// ==============

namespace gse::impl::jacobian {

// Compute jacobian of a vector function:
//    f: R^N -> R^N
//
// here:
//    'f'      - vector-function,
//    'x'      - evaluation point,
//    'method' - numerical differentiation method.
//
template <class T, Extent N, class Func, class Method = defaults::jacobian_method<T>,
          require_vector_function<T, N, Func> = true, require_valid_method<T, N, Func, Method> = true>
Matrix<T, N, N> solve(Func&& f, const Vector<T, N>& x, Method method = Method{}) {
    return method(f, x);
}

} // namespace gse::impl::jacobian
