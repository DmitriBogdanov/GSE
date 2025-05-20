// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include <limits> // numeric_limits<>::epsilon()

#include "../../core/math.hpp"
#include "../../core/types.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Default method params and method base classes.
//
// These exist purely to reduce boilerplate, instead of specifying members and their default values
// in each method separately we can just pull in a "preset" of parameters by inheriting a class.

// ____________________ IMPLEMENTATION ____________________

// ================
// --- Defaults ---
// ================

namespace gse::impl::gradient::defaults {

// Finite difference steps sizes
template <class T>
constexpr T central_diff_step = math::cbrt(std::numeric_limits<T>::epsilon()); // ~ 'h' in literature
// for central differences optimal step size is a cube root of machine epsilon,
// see https://en.wikipedia.org/wiki/Numerical_differentiation

} // namespace gse::impl::jacobian::defaults

// ===========================
// --- Method base classes ---
// ===========================

namespace gse::impl::gradient::base {

template <class T>
struct CentralScheme {
    T diff_step = defaults::central_diff_step<T>; // differentiation step
};

} // namespace gse::impl::jacobian::base
