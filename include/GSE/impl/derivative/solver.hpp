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

// Numerical derivative evaluation.
//
// Solver doesn't do much beyond wrapping the method.

// ____________________ IMPLEMENTATION ____________________

// ================
// --- Defaults ---
// ================

namespace gse::impl::derivative::defaults {

using derivative_method = method::CentralDifference;

} // namespace gse::impl::derivative::defaults

// ==============
// --- SFINAE ---
// ==============

namespace gse::impl::derivative {

template <class T, Extent N, class Func, class Method>
using require_valid_method = require_invocable_r<T, Method, Func, T>;

} // namespace gse::impl::derivative

// ==============
// --- Solver ---
// ==============

namespace gse::impl::derivative {

// Compute gradient of a multi-variate scalar function:
//    f: R^N -> R
//
// here:
//    'f'              - multi-variate scalar function,
//    'x'              - evaluation point,
//    'method'         - numerical differentiation method.
//
template <class T, Extent N, class Func, class Method = defaults::derivative_method,
          require_scalar_function<T, N, Func> = true, require_valid_method<T, N, Func, Method> = true>
T solve(Func&& f, T x, Method method = Method{}) {
    return method(f, x);
}

} // namespace gse::impl::derivative
