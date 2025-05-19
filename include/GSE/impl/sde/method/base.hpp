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
namespace gse::impl::sde::defaults {

// General
template <class T> constexpr T time_step = 1e-3; // ~ 'tau' in literature

} // namespace gse::ode::defaults
// clang-format on

// ===========================
// --- Method base classes ---
// ===========================

namespace gse::impl::sde::base {

template <class T>
struct Common {
    T time_step = defaults::time_step<T>; // time step
};

} // namespace gse::impl::ode::base
