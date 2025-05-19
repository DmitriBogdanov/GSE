// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "../../core/types.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Default integrator params and method base classes.
//
// These exist purely to reduce boilerplate, instead of specifying members and their default values
// in each method separately we can just pull in a "preset" of parameters by inheriting a class.

// ____________________ IMPLEMENTATION ____________________

// ================
// --- Defaults ---
// ================

// clang-format off
namespace gse::impl::ode::defaults {

// General
template <class T> constexpr T time_step = 1e-3; // ~ 'tau' in literature

// Implicit
template <class T> constexpr T newton_precision      = 1e-12;
template <class T> constexpr T newton_max_iterations = 100;

// Adaptive methods
template <class T> constexpr T min_time_step   = 1e-6; // ~ 'tau_max' in literature
template <class T> constexpr T max_time_step   = 1e-1; // ~ 'tau_min' in literature
template <class T> constexpr T tolerance       = 1e-6; // ~ 'tol'     in literature
template <class T> constexpr T adaptation_rate = 0.7;  // ~ 'fact'    in literature, usually 0.7 / 0.8 / 0.9
template <class T> constexpr T adaptation_min  = 0.7;  // ~ 'factmin' in literature, usually in [0.2, 0.7]
template <class T> constexpr T adaptation_max  = 1.5;  // ~ 'factmax' in literature, usually in [1.5, 5.0]

} // namespace gse::ode::defaults
// clang-format on

// ===========================
// --- Method base classes ---
// ===========================

namespace gse::impl::ode::base {

template <class T>
struct Common {
    T time_step = defaults::time_step<T>; // time step
};

template <class T>
struct Implicit : Common<T> {
    T newton_precision      = defaults::newton_precision<T>;
    T newton_max_iterations = defaults::newton_max_iterations<T>;
};

template <class T>
struct Adaptive : Common<T> {
    T min_time_step = defaults::min_time_step<T>;
    T max_time_step = defaults::max_time_step<T>;

    T tolerance       = defaults::tolerance<T>;
    T adaptation_rate = defaults::adaptation_rate<T>;
    T adaptation_min  = defaults::adaptation_min<T>;
    T adaptation_max  = defaults::adaptation_max<T>;

    T err = 0;
};

} // namespace gse::impl::ode::base
