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

namespace gse::impl::nonlinear::defaults {

template <class T>
constexpr T precision = math::sqr(math::cbrt(std::numeric_limits<T>::epsilon()));
// machine_eps^(2/3), decent general estimate for how precise we want to be,
// ~ 3.6e-11 for 'double'
// ~ 2.4e-05 for 'float'

constexpr Uint max_iterations = 100;

} // namespace gse::impl::nonlinear::defaults

// ===========================
// --- Method base classes ---
// ===========================

namespace gse::impl::nonlinear::base {

template <class T>
struct Common {
    T    precision      = defaults::precision<T>; // target iteration difference
    Uint max_iterations = defaults::max_iterations;
};

} // namespace gse::impl::nonlinear::base
