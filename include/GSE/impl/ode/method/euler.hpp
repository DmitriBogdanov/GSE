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

#include "./base.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Iteration of an ODE integration method. Explicit Euler's scheme.
//    Error:       O(tau)
//    Cost:        1 'f(t, x)' evaluation
//    Stability:   -
//    Adaptive:    false
//    Explicit:    true
//    Single-step: true
//    Stiff:       false
//    Symplectic:  false
//
// Simplest method out there. Not particularly good, but still works for some problems.

// ____________________ IMPLEMENTATION ____________________

namespace gse::impl::ode::method {

template <class T>
struct Euler : base::Common<T> {

    template <Extent N, class Func, require_time_vector_function<T, N, Func> = true>
    std::pair<T, Vector<T, N>> operator()(Func&& f, T t, Vector<T, N> y0) {
        const T tau = this->time_step;

        y0 += tau * f(t, y0);
        t += tau;

        return {t, std::move(y0)};
    }
};

} // namespace gse::impl::ode::method
