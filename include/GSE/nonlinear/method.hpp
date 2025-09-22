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
#include "GSE/impl/prototypes.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Common constrains & functionality of nonlinear methods.

// ____________________ IMPLEMENTATION ____________________

namespace gse::nonlinear {

// clang-format off

// [ LSP documentation ]
//
// Concept that constrains `Method` to a signature of a nonlinear method:
//    `Vector<T, N> ( VectorFunction, Vector<T, N> )`
//
// while ensuring that it provides necessary member variables `precision` and `max_iterations`.
//
template <class Method, class T, Extent N>
concept method_function =
    invocable_r<Method, Vector<T, N>, impl::prototypes::VectorFunction<T, N>, Vector<T, N>>
    &&
    std::same_as<T, decltype(std::declval<Method>().precision)>
    &&
    std::same_as<Uint, decltype(std::declval<Method>().max_iterations)>;

} // namespace gse::nonlinear

// clang-format on

// ===============
// --- Presets ---
// ===============

namespace gse::nonlinear::impl {

template <scalar T>
struct IterativeScheme {
    T precision = gse::impl::sqr(gse::impl::cbrt(std::numeric_limits<T>::epsilon()));
    // machine_eps^(2/3), decent general estimate for how precise we want to be,
    // ~ 3.6e-11 for 'double'
    // ~ 2.4e-05 for 'float'

    Uint max_iterations = 200;
    // reasonably limits evaluation time
};

} // namespace gse::nonlinear::impl
