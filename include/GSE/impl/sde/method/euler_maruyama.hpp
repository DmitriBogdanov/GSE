// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include <cmath>   // sqrt()
#include <utility> // pair<>

#include "../../core/init.hpp"
#include "../../core/traits.hpp"
#include "../../core/types.hpp"

#include "./base.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Iteration of an ODE integration method. Explicit Euler's scheme.
//    Weak error:   O(tau)
//    Strong error: O(tau^1/2)
//    Cost:         1 'f(t, x)' evaluation
//    Stability:    -
//    Adaptive:     false
//    Explicit:     true
//    Single-step:  true
//    Stiff:        false
//    Symplectic:   false
//
// Simplest stochastic method out there. Quite bad due to strong convergence order of 1/2
// (weak/strong convergence orders are and SDE-specific thing), but still suitable for some problems.

// ____________________ IMPLEMENTATION ____________________

namespace gse::impl::sde::method {

template <class T>
struct EulerMaruyama : base::Common<T> {

    template <Extent N, class FuncA, class FuncB, class Gen, class Dist,
              require_time_vector_function<T, N, FuncA> = true, require_time_vector_function<T, N, FuncB> = true>
    std::pair<T, Vector<T, N>> operator()(FuncA&& A, FuncB&& B, Gen& gen, Dist& dist, T t, Vector<T, N> y0) {
        const T tau      = this->time_step;
        const T sqrt_tau = std::sqrt(tau);

        const Vector<T, N> dW = sqrt_tau * init::random<T, N>(y0.rows(), dist, gen);

        y0 += tau * A(t, y0) + dW.cwiseProduct(B(t, y0));
        t += tau;

        return {t, std::move(y0)};
    }
};

} // namespace gse::impl::sde::method
