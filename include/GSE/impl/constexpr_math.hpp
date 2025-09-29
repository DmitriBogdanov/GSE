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

// ____________________ DEVELOPER DOCS ____________________

// 'constexpr' mathematical functions used for computing constant.
// Not a public header, so we don't mention it in the docs.

// ____________________ IMPLEMENTATION ____________________

namespace gse::impl {

template <scalar T>
[[nodiscard]] constexpr T abs(T x) noexcept {
    return (x > T(0)) ? x : -x;
}

// Iterative exponentiation by squaring, standard implementation,
// see https://en.wikipedia.org/wiki/Exponentiation_by_squaring
template <scalar T>
[[nodiscard]] constexpr T pow(T x, int n) noexcept {
    if (n < 0) x = T(1) / x, n = -n;

    T y = 1;

    while (n > 1) {
        if (n-- % 2) y *= x;
        x *= x;
        n /= 2;
    }

    return x * y;
}

template <scalar T>
[[nodiscard]] constexpr T sqr(T x) noexcept {
    return x * x;
}

template <scalar T>
[[nodiscard]] constexpr T cube(T x) noexcept {
    return x * x * x;
}

template <scalar T>
[[nodiscard]] constexpr bool approx_equals(T x, T y) noexcept {
    return abs(x - y) <= std::numeric_limits<T>::epsilon();
}

// Constexpr n-th root based on recursive iteration of Newton's method:
//
// Let's say we have 'x' and need to compute
//    r = x^(1/n)
// let's rewrite it as an equation 'f(r) = 0':
//    r^n = x
//    r^n - x = 0
//    => f(r) = 0 where f(r) = r^n - x, f'(r) = n r^(n-1)
// we can now compute 'r' iteratively using Newton's method:
//     r_k+1 = r_k - f(r_k) / f'(r_k)
// we assume 'r' is computed precisely enough when |r_k+1 - r_k| < machine_epsilon
//
// Note: May not handle NaN / inf edge cases, but we don't care in this context.
//
template <std::size_t n, scalar T>
[[nodiscard]] constexpr T root(T x, T r0 = T(1)) noexcept {
    const T r = r0 - (pow(r0, n) - x) / (T(n) * pow(r0, n - 1));

    return approx_equals(r0, r) ? r0 : root<n>(x, r);
}

template <scalar T>
[[nodiscard]] constexpr T sqrt(T x) noexcept {
    return root<2>(x);
}

template <scalar T>
[[nodiscard]] constexpr T cbrt(T x) noexcept {
    return root<3>(x);
}

} // namespace gse::impl
