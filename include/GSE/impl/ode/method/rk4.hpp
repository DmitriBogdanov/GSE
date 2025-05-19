// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include <utility> // pair<>

#include "../../core/types.hpp"

#include "./base.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Iteration of an ODE integration method. 4th order fixed-step Runge-Kutta scheme.
//    Error:       O(tau^4)
//    Cost:        4 'f(t, x)' evaluations
//    Stability:   -
//    Adaptive:    false
//    Explicit:    true
//    Single-step: true
//    Stiff:       false
//    Symplectic:  false
//
// Popular and well-balanced method, suitable for most non-stiff problems.

// ____________________ IMPLEMENTATION ____________________

namespace gse::impl::ode::method {

template <class T>
struct RK4 : base::Common<T> {
private:
    
    // Butcher's table
    //
    // See https://en.wikipedia.org/wiki/Runge–Kutta_methods
    //
    // Special values '0' and '1' are marked with '[!zero]' and '[!one]' comments
    // and can be effectively skipped in the computation.
    //
    constexpr static T c1 = 0.; // [!zero]
    constexpr static T c2 = 1. / 2.;
    constexpr static T c3 = 1. / 2.;
    constexpr static T c4 = 1.; // [!one]
    
    constexpr static T a21 = 1. / 2.;
    constexpr static T a31 = 0.; // [!zero]
    constexpr static T a32 = 1. / 2.;
    constexpr static T a41 = 0.; // [!zero]
    constexpr static T a42 = 0.; // [!zero]
    constexpr static T a43 = 1.; // [!one]
    
    constexpr static T b1 = 1. / 6.;
    constexpr static T b2 = 1. / 3.;
    constexpr static T b3 = 1. / 3.;
    constexpr static T b4 = 1. / 6.;
    
    // Numerator / denominator split for slightly faster computation (saves a few multiplications)
    
    constexpr static T bn1 = 1.; // [!one]
    constexpr static T bn2 = 2.;
    constexpr static T bn3 = 2.;
    constexpr static T bn4 = 1.; // [!one]
    
    constexpr static T bd = 1. / 6.;
    
public:
    
    template <Extent N, class Func>
    std::pair<T, Vector<T, N>> operator()(Func&& f, T t, Vector<T, N> y0) {
        const T tau = this->time_step;
        
        const Vector<T, N> k1 = f(t, y0);
        const Vector<T, N> k2 = f(t + tau * c2, y0 + tau * a21 * k1);
        const Vector<T, N> k3 = f(t + tau * c3, y0 + tau * a32 * k2);
        const Vector<T, N> k4 = f(t + tau, y0 + tau * k3);
        
        y0 += tau * bd * (k1 + bn2 * k2 + bn3 * k3 + k4);
        t += tau;
        
        return { t, std::move(y0) };
    }
};

} // namespace gse::XXXXXXXXXXXX
