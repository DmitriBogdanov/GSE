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

#include "../../core/traits.hpp"
#include "../../core/types.hpp"
#include "../../nonlinear/solver.hpp"

#include "./base.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Iteration of an ODE integration method. Implicit Euler's scheme.
//    Error:       O(tau)
//    Cost:        1 nonlinear NxN system with 1 internal 'f()' evaluation
//    Stability:   L
//    Adaptive:    false
//    Explicit:    false
//    Single-step: true
//    Stiff:       true
//    Symplectic:  false
//
// The simplest stiff method. Not particularly good, but already L-stable.

// ____________________ IMPLEMENTATION ____________________

namespace gse::impl::ode::method {

template <class T, class NonlinearMethod = nonlinear::defaults::nonlinear_method<T>>
struct ImplicitEuler : base::Common<T> {

    NonlinearMethod nonlinear_method;
    
    template <Extent N, class Func, require_time_vector_function<T, N, Func> = true>
    std::pair<T, Vector<T, N>> operator()(Func&& f, T t, Vector<T, N> y0) {
        const T tau = this->time_step;

        const auto implicit_equation = [&](const Vector<T, N>& y1) -> Vector<T, N> {
            return y1 - y0 - tau * f(t + tau, y1);
        };

        y0 = nonlinear::solve(implicit_equation, std::move(y0), this->nonlinear_method);
        t += tau;
        return {t, std::move(y0)};
    }
};

} // namespace gse::impl::ode::method
