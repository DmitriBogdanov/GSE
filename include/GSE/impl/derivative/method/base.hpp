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

// ____________________ DEVELOPER DOCS ____________________

// Default method params and method base classes.
//
// These exist purely to reduce boilerplate, instead of specifying members and their default values
// in each method separately we can just pull in a "preset" of parameters by inheriting a class.

// ____________________ IMPLEMENTATION ____________________

// ================
// --- Defaults ---
// ================

namespace gse::impl::derivative::defaults {

// Finite difference steps sizes
template <class T>
constexpr T directional_diff_step = math::root<2>(std::numeric_limits<T>::epsilon()); // ~ 'h' in literature
// 2-nd of machine epsilon is a good estimate for most cases,
// see https://en.wikipedia.org/wiki/Numerical_differentiation

template <class T>
constexpr T central_diff_step = math::root<3>(std::numeric_limits<T>::epsilon()); // ~ 'h' in literature
// 3-rd of machine epsilon is a good estimate for most cases,
// see https://en.wikipedia.org/wiki/Numerical_differentiation

template <class T>
constexpr T four_point_central_diff_step = math::root<5>(std::numeric_limits<T>::epsilon()); // ~ 'h' in literature
// 5-th of machine epsilon is a good estimate for most cases,
// see https://math.stackexchange.com/questions/2019573

} // namespace gse::impl::derivative::defaults

// ===========================
// --- Method base classes ---
// ===========================

namespace gse::impl::derivative::base {

template <class T>
struct DirectionalScheme {
    T diff_step = defaults::directional_diff_step<T>;
};

template <class T>
struct CentralScheme {
    T diff_step = defaults::central_diff_step<T>;
};

template <class T>
struct FourPointCentralScheme {
    T diff_step = defaults::four_point_central_diff_step<T>;
};

} // namespace gse::impl::derivative::base
