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
#include "../../nonlinear/solver.hpp"

#include "./base.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Iteration of an ODE integration method. Symplectic Euler's scheme.
// Also known as Symmetric Euler or Trapezoidal Rule.
//    Error:       O(tau^2)
//    Cost:        1 nonlinear NxN system with 2 internal 'f()' evaluations
//    Stability:   A
//    Adaptive:    false
//    Explicit:    false
//    Single-step: true
//    Stiff:       true
//    Symplectic:  true
//
// The simplest stiff 2nd order method. Has a nice property of being symplectic due to symmetry.

// ____________________ IMPLEMENTATION ____________________

namespace gse::impl::ode::method {

template <class T, class NonlinearMethod = nonlinear::defaults::nonlinear_method>
struct SymplecticEuler : base::Implicit<T> {

    template <Extent N, class Func>
    std::pair<T, Vector<T, N>> operator()(Func&& f, T t, Vector<T, N> y0) {
        const T tau = this->time_step;

        const auto implicit_equation = [&](const Vector<T, N>& y1) -> Vector<T, N> {
            return y1 - y0 - tau * 0.5 * (f(t + tau, y1) + f(t, y0));
        };

        y0 = nonlinear::solve(implicit_equation, std::move(y0), this->newton_precision, this->newton_max_iterations,
                              NonlinearMethod{});
        t += tau;
        return {t, std::move(y0)};
    }
};

} // namespace gse::impl::ode::method
