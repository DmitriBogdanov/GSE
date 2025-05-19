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

// Numerical gradient evaluation.
//
// Solver doesn't do much beyond wrapping the method.

// ____________________ IMPLEMENTATION ____________________

// ================
// --- Defaults ---
// ================

namespace gse::impl::gradient::defaults {

using gradient_method = method::CentralDifference;

} // namespace gse::impl::gradient::defaults

// ==============
// --- SFINAE ---
// ==============

namespace gse::impl::gradient {

template <class T, Extent N, class Func, class Method>
using require_valid_method = require_invocable_r<Vector<T, N>, Method, Func, Vector<T, N>>;

} // namespace gse::impl::gradient

// ==============
// --- Solver ---
// ==============

namespace gse::impl::gradient {

// Compute gradient of a multi-variate scalar function:
//    f: R^N -> R
//
// here:
//    'f'              - multi-variate scalar function,
//    'x'              - evaluation point,
//    'method'         - numerical differentiation method.
//
template <class T, Extent N, class Func, class Method = defaults::gradient_method,
          require_multivariate_function<T, N, Func> = true, require_valid_method<T, N, Func, Method> = true>
Vector<T, N> solve(Func&& f, const Vector<T, N>& x, Method method = Method{}) {
    return method(f, x);
}

} // namespace gse::impl::gradient
