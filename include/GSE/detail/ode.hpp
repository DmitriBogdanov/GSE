// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include <cmath>     // isfinite()
#include <stdexcept> // runtime_error
#include <string>    // to_string
#include <type_traits>

#include "alg.hpp"
#include "core.hpp"

// ____________________ DEVELOPER DOCS ____________________

// TODO: DOCS

// ____________________ IMPLEMENTATION ____________________

// =======================
// --- ODE Integrators ---
// =======================

namespace gse::ode::integrators {

// --- Commonly used integration formulas ---
// ------------------------------------------

template <Extent N, class Func>
void _rk4_step(Vector<N>& k1, Vector<N>& k2, Vector<N>& k3, Vector<N>& k4, Vector<N>& tmp, // reusable storage
               Scalar t, Vector<N>& y0, Func&& f, Scalar tau                               // problem
) {
    k1  = f(t, y0);
    tmp = y0 + 0.5 * tau * k1;
    k2  = f(t + 0.5 * tau, tmp);
    tmp = y0 + 0.5 * tau * k2;
    k3  = f(t + 0.5 * tau, tmp);
    tmp = y0 + tau * k3;
    k4  = f(t + tau, tmp);

    y0 += tau / 6. * (k1 + 2. * k2 + 2. * k3 + k4);

    t += tau;
}

// --- Integrators ---
// -------------------

constexpr Scalar default_tau       = 1e-3;
constexpr Scalar default_tau_min   = 1e-9;
constexpr Scalar default_tau_max   = 1e-1;
constexpr Scalar default_tolerance = std::numeric_limits<Scalar>::epsilon();

// Euler
// > Euler's explicit method
// > Explicit, O(tau)
template <Extent N = dynamic_size>
struct Euler {
    // - Params -
    Scalar tau = default_tau;

    // - Integrator -
    template <class Func>
    void operator()(Scalar& t, Vector<N>& y0, Func&& f) {
        y0 += tau * f(t, y0);

        t += tau;
    }
};

// RK4
// > 4-th order Runge-Kutta method
// > Explicit, O(tau^4)
template <Extent N = dynamic_size>
struct RK4 {
    // - Params -
    Scalar tau = default_tau;

    // - Integrator -
    template <class Func>
    void operator()(Scalar& t, Vector<N>& y0, Func&& f) {
        // k1 = f(t, y0);
        // k2 = f(t + 0.5 * tau, y0 + 0.5 * tau * k1);
        // k3 = f(t + 0.5 * tau, y0 + 0.5 * tau * k2);
        // k4 = f(t + tau, y0 + tau * k3);

        // y0 += tau / 6. * (k1 + 2. * k2 + 2. * k3 + k4);

        _rk4_step(k1, k2, k3, k4, tmp, t, y0, f, tau);

        t += tau;
    }

private:
    Vector<N> k1, k2, k3, k4;
    Vector<N> tmp;
};

// AdamsRK4
// > 4-th order Adams method with Runge-Kutta initialization
// > Explicit, 4-step, O(tau^4)
template <Extent N = dynamic_size>
struct AdamsRK4 {
    // - Params -
    Scalar tau = default_tau;

    // - Integrator -
    template <class Func>
    void operator()(Scalar& t, Vector<N>& y0, Func&& f) {
        // First 4 iterations are obtained through RK4
        if (iteration < 4) {
            k1 = f(t, y0);
            k2 = f(t + 0.5 * tau, y0 + 0.5 * tau * k1);
            k3 = f(t + 0.5 * tau, y0 + 0.5 * tau * k2);
            k4 = f(t + tau, y0 + tau * k3);

            y0 += tau / 6. * (k1 + 2. * k2 + 2. * k3 + k4);

            if (iteration == 0) fm4 = k1;
            else if (iteration == 1) fm3 = k1;
            else if (iteration == 2) fm2 = k1;
            else if (iteration == 3) fm1 = k1;

            ++iteration;
        }
        // After that we switch to Adams4
        else {
            fm4 = fm3;
            fm3 = fm2;
            fm2 = fm1;
            fm1 = f(t, y0);

            y0 += tau / 24. * (55. * fm1 - 59. * fm2 + 37. * fm3 - 9. * fm4);
        }

        t += tau;
    }

private:
    Uint      iteration = 0;
    Vector<N> k1, k2, k3, k4;
    Vector<N> fm1, fm2, fm3, fm4;
};

// RK4RE
// > 4-th order Runge-Kutta method with Richardson Extrapolation
// > Explicit, O(tau^4)
template <Extent N = dynamic_size>
struct RK4RE {
    // - Params -
    Scalar tau     = default_tau;
    Scalar tau_min = default_tau_min; // hard min on 'tau' adaptaiton
    Scalar tau_max = default_tau_max; // hard max on 'tau' adaptaiton

    Scalar tolerance = default_tolerance; // error tolerance
    Scalar fact      = 0.7;               // usually 0.7 / 0.8 / 0.9 / 0.95, measure of our confidence in extrapolation
    Scalar factmin   = 0.7;               // usually in [0.2, 0.7] range, limits how fast 'tau' can shrink
    Scalar factmax   = 1.5;               // usually in [1.5, 5.0] range, limits how fast 'tau' can grow

    // - Integrator -
    template <class Func>
    void operator()(Scalar& t, Vector<N>& y0, Func&& f) {

        Scalar err{};

        do {
            // One double-width step
            w = y0;
            _rk4_step(k1, k2, k3, k4, tmp, t, w, f, 2. * tau);

            // Two single-width steps
            y2 = y0;
            _rk4_step(k1, k2, k3, k4, tmp, t, y2, f, tau);
            _rk4_step(k1, k2, k3, k4, tmp, t + tau, y2, f, tau);

            // Error estimate
            Scalar err = 0; // = 1 / (2^p - 1) * max{...}
            for (Idx i = 0; i < y0.size(); ++i) err = std::max(err, (y2[i] - w[i]) / std::abs(y2[i]));
            err *= 1. / (16. - 1.);

            // Step correction
            const Scalar tau_growth_factor = fact * (tolerance / err);
            tau *= std::clamp(tau_growth_factor, factmin, factmax);
            tau = std::clamp(tau, tau_min, tau_max);
        } while (err >= tolerance);

        // y0 = y2;                          // regular step
        // y0 = y2 + (y2 - w) / (2 * p - 1); // (p + 1)-order approximation using a theorem
        y0 = y2 + (y2 - w) / (2. * 4 - 1);

        t += 2 * tau;
    }

private:
    Vector<N> k1, k2, k3, k4;
    Vector<N> tmp;
    Vector<N> w, y2;
};

} // namespace gse::ode::integrators

// ==================
// --- ODE Solver ---
// ==================

namespace gse::ode {

// Required callable signatures:
//    > Vector<N> f(Scalar t, const Vector<N>& y0)
//    > void callback(Scalar t, const Vector<N>& y0, const Integrator& integrator)
//    > void integrator(Scalar& t, Vector<N>& y0, const Func& f)
// Note 1: The SFINAE is ugly, but it makes user API more robust.
// Note 2: See 'guide_passing_callables.md'
template <Extent N,                                 //
          class Func,                               //
          class Callback,                           //
          class Integrator = integrators::RK4RE<N>, //

          // Enforce function signatures
          _require_invocable_r<Vector<N>, Func, Scalar, Vector<N>>                  = true, //
          _require_invocable<Callback, Scalar, Vector<N>, std::decay_t<Integrator>> = true, //
          _require_invocable<Integrator, Scalar&, Vector<N>&, std::decay_t<Func>>   = true  //

          >
void solve(Func&&       f,                         // system RHS
           Vector<N>    y0,                        // initial condition
           Scalar       t0,                        // time interval start
           Scalar       t1,                        // time interval end
           Callback&&   callback,                  // callback to export the result
           Scalar       callback_frequency,        // how often to call 'callback'
           Integrator&& integrator = Integrator{}, // integration method
           bool         verify     = true          // whether to test for divergence
) {
    Scalar t                = t0;
    Scalar t_prev           = 0;
    Scalar t_since_callback = 0;

    const auto test_solution_for_divergence = [&] {
        for (Idx idx = 0; idx < y0.size(); ++idx) {
            const Scalar val = y0[idx];
            if (std::isfinite(val)) continue;

            throw std::runtime_error("ODE solver diverges at { progress = " + std::to_string((t - t0) / (t1 - t0)) +
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
        integrator(t, y0, f);
        t_since_callback += t - t_prev;

        if (verify) test_solution_for_divergence();
        handle_callback();
    }
}

} // namespace gse::ode
