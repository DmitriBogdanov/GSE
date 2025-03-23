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
#include <random>    // mt19937, normal_distribution<>
#include <stdexcept> // runtime_error
#include <string>    // to_string
#include <type_traits>

#include "core.hpp"

// ____________________ DEVELOPER DOCS ____________________

// TODO: DOCS

// ____________________ IMPLEMENTATION ____________________

// =======================
// --- SDE Integrators ---
// =======================

namespace gse::sde::integrators {

// --- Integrators ---
// -------------------

constexpr Scalar default_tau = 1e-3;

// EulerMaruyama
// > Euler-Maruyama explicit method
// > Explicit, weak convergence O(tau), strong convergence O(tau^1/2)
template <Extent N = dynamic_size>
struct EulerMaruyama {
    // - Params -
    Scalar tau = default_tau;

    // - Integrator -
    template <class Func, class Gen, class Dist>
    void operator()(Scalar& t, Vector<N>& y0, Func&& A, Func&& B, Gen& gen, Dist& dist) {
        for (auto& e : dW) e = dist(gen);
        dW *= std::sqrt(tau);

        y0 += tau * A(t, y0) + dW.cwiseProduct(B(t, y0));

        t += tau;
    }

private:
    Vector<N> dW;
};

} // namespace gse::sde::integrators

// ==================
// --- SDE Solver ---
// ==================

namespace gse::sde {

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
          class Integrator = integrators::EulerMaruyama<N>,    //
          class Gen        = std::mt19937,                     //
          class Dist       = std::normal_distribution<Scalar>, //

          // Enforce function signatures
          _require_invocable_r<Vector<N>, FuncA, Scalar, Vector<N>>                 = true, //
          _require_invocable_r<Vector<N>, FuncB, Scalar, Vector<N>>                 = true, //
          _require_invocable<Callback, Scalar, Vector<N>, std::decay_t<Integrator>> = true, //
          _require_invocable<Integrator, Scalar&, Vector<N>&, std::decay_t<FuncA>, std::decay_t<FuncA>,
                             std::decay_t<Gen>, std::decay_t<Dist>>                 = true //

          >
void solve(FuncA&&      A,                         // deterministic part of system RHS
           FuncB&&      B,                         // stochastic part of system RHS
           Vector<N>    y0,                        // initial condition
           Scalar       t0,                        // time interval start
           Scalar       t1,                        // time interval end
           Callback&&   callback,                  // callback to export the result
           Scalar       callback_frequency,        // how often to call 'callback'
           Integrator&& integrator = Integrator{}, // method
           bool         verify     = true,         // whether to test for divergence
           Gen&&        gen        = Gen{},        // underlying PRNG
           Dist&&       dist       = Dist{}        // distribution object
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

    const auto handle_callback = [&] {
        if (t_since_callback < callback_frequency) return;
        t_since_callback -= callback_frequency;
        callback(t, y0, integrator);
    };

    // Iteration 0 (initial state)
    if (verify) test_solution_for_divergence(); // don't wanna pay for optional things even a little
    handle_callback();

    // Iterations 1...M (integration)
    while (t < t1) {
        t_prev = t;
        integrator(t, y0, A, B, gen, dist);
        t_since_callback += t - t_prev;

        if (verify) test_solution_for_divergence();
        handle_callback();
    }
}

} // namespace gse::sde
