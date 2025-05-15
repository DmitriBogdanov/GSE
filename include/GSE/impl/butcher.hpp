// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "types.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Butcher Tables & step functions of various Runge-Kutta methods.
//
// Special values '0' and '1' are marked with '[!zero]' and '[!one]' comments
// and can be effectively skipped in the computation.
//
// Coefficients 'b1' ... 'bS' are split into numerators ('bn1' ... 'bnS')
// and general denominator ('bd') since writing expressions that way results in less multiplications.

// ____________________ IMPLEMENTATION ____________________

// =====================
// --- Runge-Kutta 4 ---
// =====================

// See https://en.wikipedia.org/wiki/Runge–Kutta_methods

namespace gse::impl::butcher::rk4 {

constexpr Scalar c1 = 0.; // [!zero]
constexpr Scalar c2 = 1. / 2.;
constexpr Scalar c3 = 1. / 2.;
constexpr Scalar c4 = 1.; // [!one]

constexpr Scalar a21 = 1. / 2.;
constexpr Scalar a31 = 0.; // [!zero]
constexpr Scalar a32 = 1. / 2.;
constexpr Scalar a41 = 0.; // [!zero]
constexpr Scalar a42 = 0.; // [!zero]
constexpr Scalar a43 = 1.; // [!one]

constexpr Scalar b1 = 1. / 6.;
constexpr Scalar b2 = 1. / 3.;
constexpr Scalar b3 = 1. / 3.;
constexpr Scalar b4 = 1. / 6.;

// Numerator / denominator split for slightly faster computation

constexpr Scalar bn1 = 1.; // [!one]
constexpr Scalar bn2 = 2.;
constexpr Scalar bn3 = 2.;
constexpr Scalar bn4 = 1.; // [!one]

constexpr Scalar bd = 1. / 6.;

template <Extent N, class Func>
Vector<N> step(Func&& f, Scalar t, Vector<N>& y0, Scalar tau) {

    const Vector<N> k1 = f(t, y0);
    const Vector<N> k2 = f(t + tau * c2, y0 + tau * a21 * k1);
    const Vector<N> k3 = f(t + tau * c3, y0 + tau * a32 * k2);
    const Vector<N> k4 = f(t + tau, y0 + tau * k3);

    // return y0 + tau * (b1 * k1 + b2 * k2 + b3 * k3 + b4 * k4);
    return y0 + tau * bd * (k1 + bn2 * k2 + bn3 * k3 + k4);
}

} // namespace gse::impl::butcher::rk4

// ===========================
// --- Dormand-Prince 4(5) ---
// ===========================

// See https://en.wikipedia.org/wiki/Dormand–Prince_method

namespace gse::impl::butcher::dopri45 {

constexpr Scalar c1 = 0.; // [!zero]
constexpr Scalar c2 = 1. / 5.;
constexpr Scalar c3 = 3. / 10.;
constexpr Scalar c4 = 4. / 5.;
constexpr Scalar c5 = 8. / 9.;
constexpr Scalar c6 = 1.; // [!one]
constexpr Scalar c7 = 1.; // [!one]

constexpr Scalar a21 = 1. / 5.;
constexpr Scalar a31 = 3. / 40.;
constexpr Scalar a32 = 9. / 40.;
constexpr Scalar a41 = 44. / 45.;
constexpr Scalar a42 = -56. / 15.;
constexpr Scalar a43 = 32. / 9.;
constexpr Scalar a51 = 19372. / 6561.;
constexpr Scalar a52 = -25360. / 2187.;
constexpr Scalar a53 = 64448. / 6561.;
constexpr Scalar a54 = -212. / 729.;
constexpr Scalar a61 = 9017 / 3168;
constexpr Scalar a62 = -355. / 33.;
constexpr Scalar a63 = 46732. / 5247.;
constexpr Scalar a64 = 49. / 176.;
constexpr Scalar a65 = -5103. / 18656.;
constexpr Scalar a71 = 35. / 384.;
constexpr Scalar a72 = 0.; // [!zero]
constexpr Scalar a73 = 500. / 1113.;
constexpr Scalar a74 = 125. / 192.;
constexpr Scalar a75 = -2187. / 6784.;
constexpr Scalar a76 = 11. / 84.;

// 'b1 .. bS' for embedded 4-th order method
// ('e' stands for 'embedded')
constexpr Scalar be1 = a71;
constexpr Scalar be2 = 0.; // [!zero]
constexpr Scalar be3 = a73;
constexpr Scalar be4 = a74;
constexpr Scalar be5 = a75;
constexpr Scalar be6 = a76;
constexpr Scalar be7 = 0.; // [!zero]

constexpr Scalar b1 = 5179. / 57600.;
constexpr Scalar b2 = 0.; // [!zero]
constexpr Scalar b3 = 7571. / 16695.;
constexpr Scalar b4 = 393. / 640.;
constexpr Scalar b5 = -92097. / 339200.;
constexpr Scalar b6 = 187. / 2100.;
constexpr Scalar b7 = 1. / 40.;

template <Extent N, class Func>
Vector<N> step(Func&& f, Scalar t, const Vector<N>& y0, Scalar tau) {

    const Vector<N> k1 = f(t, y0);
    const Vector<N> k2 = f(t + tau * c2, y0 + tau * (a21 * k1));
    const Vector<N> k3 = f(t + tau * c3, y0 + tau * (a31 * k1 + a32 * k2));
    const Vector<N> k4 = f(t + tau * c4, y0 + tau * (a41 * k1 + a42 * k2 + a43 * k3));
    const Vector<N> k5 = f(t + tau * c5, y0 + tau * (a51 * k1 + a52 * k2 + a53 * k3 + a54 * k4));
    const Vector<N> k6 = f(t + tau, y0 + tau * (a61 * k1 + a62 * k2 + a63 * k3 + a64 * k4 + a65 * k5));
    const Vector<N> k7 = f(t + tau, y0 + tau * (a71 * k1 + a73 * k3 + a74 * k4 + a75 * k5 + a76 * k6));

    return y0 + tau * (b1 * k1 + b3 * k3 + b4 * k4 + b5 * k5 + b6 * k6 + b7 * k7);
}

template <Extent N, class Func>
std::pair<Vector<N>, Vector<N>> embedded_step(Func&& f, Scalar t, const Vector<N>& y0, Scalar tau) {

    const Vector<N> k1 = f(t, y0);
    const Vector<N> k2 = f(t + tau * c2, y0 + tau * (a21 * k1));
    const Vector<N> k3 = f(t + tau * c3, y0 + tau * (a31 * k1 + a32 * k2));
    const Vector<N> k4 = f(t + tau * c4, y0 + tau * (a41 * k1 + a42 * k2 + a43 * k3));
    const Vector<N> k5 = f(t + tau * c5, y0 + tau * (a51 * k1 + a52 * k2 + a53 * k3 + a54 * k4));
    const Vector<N> k6 = f(t + tau, y0 + tau * (a61 * k1 + a62 * k2 + a63 * k3 + a64 * k4 + a65 * k5));
    const Vector<N> k7 = f(t + tau, y0 + tau * (a71 * k1 + a73 * k3 + a74 * k4 + a75 * k5 + a76 * k6));

    return {
        y0 + tau * (be1 * k1 + be3 * k3 + be4 * k4 + be5 * k5 + be6 * k6),
        y0 + tau * (b1 * k1 + b2 * k2 + b3 * k3 + b4 * k4 + b5 * k5 + b6 * k6 + b7 * k7),
    };
}

} // namespace gse::impl::butcher::dopri45