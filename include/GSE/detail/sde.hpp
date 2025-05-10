// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include <cmath>     // sqrt(), isfinite()
#include <random>    // normal_distribution<>
#include <stdexcept> // runtime_error
#include <string>    // to_string

#include "core.hpp"
#include "rng.hpp"
#include "traits.hpp"

// ____________________ DEVELOPER DOCS ____________________

// SDE (Stochastic Differential Equation) params, integrators and a solver-function.
//
// Some signatures may seem unwieldy with all the templates, but such is the price of being generic, in the
// end we get a very nice, flexible and completely modular API with no unnecessary overhead baked into it.
// All the convenience of Matlab-like API with full flexibility intact.

// ____________________ IMPLEMENTATION ____________________

// ====================
// --- SDE Defaults ---
// ====================

namespace gse::sde::defaults {

constexpr Scalar tau = 1e-3;

} // namespace gse::sde::defaults

// =======================
// --- SDE Integrators ---
// =======================

namespace gse::sde::integrators {

// --- Base Classes ---
// --------------------

template <Extent N = dynamic_size>
struct Base {
    Scalar tau = defaults::tau; // time step
};

// --- Usable Integrators ---
// --------------------------

// EulerMaruyama
// > Euler-Maruyama explicit method
// > Explicit, weak convergence O(tau), strong convergence O(tau^1/2)
template <Extent N = dynamic_size>
struct EulerMaruyama : Base<N> {

    template <class Func, class Gen, class Dist>
    void operator()(Func&& A, Func&& B, Gen& gen, Dist& dist, Scalar& t, Vector<N>& y0) {
        Vector<N> dW;
        for (auto& e : dW) e = dist(gen);
        dW *= std::sqrt(this->tau);

        y0 += this->tau * A(t, y0) + dW.cwiseProduct(B(t, y0));
        t += this->tau;
    }
};

// ModifiedMilstein
// > Milstein explicit method, modification from
//   [P.E. Kloeden, E. Platen] Numerical solution of stochastic differential equations (page 374)
// > Explicit, weak convergence O(tau), strong convergence O(tau)
template <Extent N = dynamic_size>
struct ModifiedMilstein : Base<N> {

    template <class Func, class Gen, class Dist>
    void operator()(Func&& A, Func&& B, Gen& gen, Dist& dist, Scalar& t, Vector<N>& y0) {
        const Scalar sqrt_of_tau = std::sqrt(this->tau);

        Vector<N> dW;
        for (auto& e : dW) e = dist(gen);
        dW *= sqrt_of_tau;

        const Vector<N> Y = y0 + this->tau * A(t, y0) + sqrt_of_tau * B(t, y0);

        y0 += this->tau * A(t, y0) + dW.cwiseProduct(B(t, y0)) +
              0.5 * (B(t, Y) - B(t, y0)).cwiseProduct(dW.cwiseAbs2() - this->tau) / sqrt_of_tau;
        t += this->tau;
    }
};

} // namespace gse::sde::integrators

// ==================
// --- SDE Solver ---
// ==================

namespace gse::sde {

// Type-erased functions
using RHS        = std::function<Vector<>(Scalar, Vector<>)>;
using Integrator = std::function<void(RHS f, Scalar& t, Vector<>& y0)>;
using Callback   = std::function<void(Scalar t, Vector<> y0, Integrator f)>;

// Required callable signatures:
//    > Vector<N> A(Scalar t, const Vector<N>& y0)
//    > Vector<N> B(Scalar t, const Vector<N>& y0)
//    > void callback(Scalar t, const Vector<N>& y0, const Integrator& integrator)
//    > void integrator(Scalar& t, Vector<N>& y0, const Func& f)
// Note 1: The SFINAE is ugly, but it makes user API more robust.
// Note 2: See 'guide_passing_callables.md'
template <Extent N,                                            //
          class FuncA,                                         //
          class FuncB,                                         //
          class Callback,                                      //
          class Integrator = integrators::ModifiedMilstein<N>, //
          class Gen        = rng::PRNG,                        //
          class Dist       = std::normal_distribution<Scalar>, //

          // Enforce function signatures
          impl::require_invocable_r<Vector<N>, FuncA, Scalar, Vector<N>>                 = true, //
          impl::require_invocable_r<Vector<N>, FuncB, Scalar, Vector<N>>                 = true, //
          impl::require_invocable<Callback, Scalar, Vector<N>, std::decay_t<Integrator>> = true, //
          impl::require_invocable<Integrator, Scalar&, Vector<N>&, std::decay_t<FuncA>, std::decay_t<FuncA>,
                                  std::decay_t<Gen>, std::decay_t<Dist>>                 = true //

          >
void solve(FuncA&&      A,                         // system RHS (deterministic part)
           FuncB&&      B,                         // system RHS (stochastic part)
           Vector<N>    y0,                        // initial condition
           Scalar       t0,                        // time interval start
           Scalar       t1,                        // time interval end
           Callback&&   callback,                  // callback to export the result
           Scalar       callback_frequency,        // how often to call 'callback'
           Integrator&& integrator = Integrator{}, // integration method
           bool         verify     = true,         // whether to test for divergence
           Gen&&        gen        = Gen{},        // underlying PRNG
           Dist&&       dist       = Dist{}        // underlying normal distribution
) {
    Scalar t                = t0;
    Scalar t_prev           = 0;
    Scalar t_since_callback = 0;

    const auto test_solution_for_divergence = [&] {
        for (Idx idx = 0; idx < y0.size(); ++idx) {
            const Scalar val = y0[idx];
            if (std::isfinite(val)) continue;

            throw std::runtime_error("SDE solver diverges at { progress = " + std::to_string((t - t0) / (t1 - t0)) +
                                     "%, t = " + std::to_string(t) + ", idx = " + std::to_string(idx) + " / " +
                                     std::to_string(y0.size()) + " }.");
        }
    };

    // Iteration 0 (initial state)
    if (verify) test_solution_for_divergence(); // don't wanna pay for optional things even a little
    callback(t, y0, integrator);

    // Iterations 1...M (integration)
    while (t < t1) {
        t_prev = t;
        integrator(A, B, gen, dist, t, y0);
        t_since_callback += t - t_prev;

        if (verify) test_solution_for_divergence();

        // Handle callback
        if (t_since_callback >= callback_frequency) {
            t_since_callback -= callback_frequency;
            callback(t, y0, integrator);
        }
    }
}

} // namespace gse::sde
