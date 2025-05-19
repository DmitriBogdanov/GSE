// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "./traits.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Compile-time math, useful for computing step sizes and mathematical constants.

// ____________________ IMPLEMENTATION ____________________

namespace gse::impl::math {

template <class T, require_arithmetic<T> = true>
[[nodiscard]] constexpr T abs(T x) noexcept {
    return (x > T(0)) ? x : -x;
}

template <class T, require_arithmetic<T> = true>
[[nodiscard]] constexpr T sqr(T x) noexcept {
    return x * x;
}

template <class T, require_arithmetic<T> = true>
[[nodiscard]] constexpr T cube(T x) noexcept {
    return x * x * x;
}

template <class T, require_floating_point<T> = true>
[[nodiscard]] constexpr bool approx_equals(T x, T y) noexcept {
    return abs(x - y) <= std::numeric_limits<T>::epsilon();
}

// Constexpr 'cbrt()' based on recursive iteration of Newton's method:
//
// Let's say we have 'x' and need to compute
//    r = x^(1/3)
// let's rewrite it as an equation 'f(r) = 0':
//    r^3 = x
//    r^3 - x = 0
//    => f(r) = 0 where f(r) = r^3 - x, f'(r) = 3 r^2
// we can now compute 'r' iteratively using Newton's method:
//     r_n+1 = r_n - f(r_n) / f'(r_n)
// we assume 'r' is computed precisely enough when |r_n+1 - r_n| < machine_epsilon
//
template <class T, require_floating_point<T> = true>
[[nodiscard]] constexpr T cbrt(T x, T r0 = T(1)) noexcept {
    const T r = r0 - (cube(r0) - x) / (T(3) * sqr(r0));

    return approx_equals(r0, r) ? r0 : cbrt(x, r);
}

} // namespace gse::impl::math
