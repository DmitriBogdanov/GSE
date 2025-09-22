// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "GSE/core/concepts.hpp"
#include "GSE/impl/constexpr_math.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Common constrains & functionality of nonlinear methods.

// ____________________ IMPLEMENTATION ____________________

namespace gse::jacobian {

// [ LSP documentation ]
//
// Concept that constrains `Method` to a signature of a jacobian method:
//    `Matrix<T, N> ( Func, Vector<T, N> )`
//
// where `Func` is a vector function.
//
template <class Method, class Func, class T, Extent N>
concept method_function = invocable_r<Method, Matrix<T, N>, Func, Vector<T, N>> && vector_function<Func, T, N>;

} // namespace gse::jacobian

// ===============
// --- Presets ---
// ===============

namespace gse::jacobian::impl {

template <scalar T>
struct CentralScheme {
    T diff_step = gse::impl::root<3>(std::numeric_limits<T>::epsilon());
    // ~ 'h' in literature, 3-rd of machine epsilon is a good estimate for most cases,
    // see https://en.wikipedia.org/wiki/Numerical_differentiation
};

} // namespace gse::jacobian::impl
