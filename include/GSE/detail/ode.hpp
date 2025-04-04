// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include <cmath> // isfinite()
#include <functional>
#include <stdexcept> // runtime_error
#include <string>    // to_string
#include <type_traits>

#include "alg.hpp"
#include "butcher.hpp"
#include "core.hpp"

#include <iostream> // TEMP:

// ____________________ DEVELOPER DOCS ____________________

// TODO: DOCS

// ____________________ IMPLEMENTATION ____________________

// ====================
// --- ODE Defaults ---
// ====================

namespace gse::ode::defaults {
    
constexpr Scalar tau       = 1e-3;
constexpr Scalar tau_min   = 1e-9;
constexpr Scalar tau_max   = 1e-1;
constexpr Scalar tolerance = 1e-12;
constexpr Scalar fact      = 0.7;
constexpr Scalar factmin   = 0.7;
constexpr Scalar factmax   = 1.5;

}

// =======================
// --- ODE Integrators ---
// =======================

namespace gse::ode::integrators {

// --- Base Classes ---
// --------------------

template <Extent N = dynamic_size>
struct Base {
    Scalar tau = defaults::tau; // time step
};

template <Extent N = dynamic_size>
struct AdaptiveBase : Base<N> {
    Scalar tau_min = defaults::tau_min; // hard min on 'tau' adaptation
    Scalar tau_max = defaults::tau_max; // hard max on 'tau' adaptation

    Scalar tolerance = defaults::tolerance; // error tolerance
    Scalar fact      = defaults::fact;      // usually 0.7 / 0.8 / 0.9, measure of our confidence in error estimate
    Scalar factmin   = defaults::factmin;   // usually in [0.2, 0.7] range, limits how fast 'tau' can shrink
    Scalar factmax   = defaults::factmax;   // usually in [1.5, 5.0] range, limits how fast 'tau' can grow
    
    gse::Uint steps_taken     = 0;
    gse::Uint steps_discarded = 0;
};

// --- Usable Integrators ---
// --------------------------

// Euler
// > Euler's explicit method
// > Explicit, O(tau)
template <Extent N = dynamic_size>
struct Euler : Base<N> {

    template <class Func>
    void operator()(Func&& f, Scalar& t, Vector<N>& y0) {
        y0 += this->tau * f(t, y0);
        t += this->tau;
    }
};

// RK4
// > 4-th order Runge-Kutta method
// > Explicit, O(tau^4)
template <Extent N = dynamic_size>
struct RK4 : Base<N> {

    template <class Func>
    void operator()(Func&& f, Scalar& t, Vector<N>& y0) {
        y0 = butcher::rk4::step(f, t, y0, this->tau);
        t += this->tau;
    }
};

// AdamsRK4
// > 4-th order Adams method with Runge-Kutta initialization
// > Explicit, 4-step, O(tau^4)
template <Extent N = dynamic_size>
struct AdamsRK4 : Base<N> {

    template <class Func>
    void operator()(Func&& f, Scalar& t, Vector<N>& y0) {
        // First 4 iterations are obtained through RK4
        if (this->iteration < 4) {
            y0 = butcher::rk4::step(f, t, y0, this->tau);
            t += this->tau;

            if (this->iteration == 0) this->fm4 = f(t, y0);
            if (this->iteration == 1) this->fm3 = f(t, y0);
            if (this->iteration == 2) this->fm2 = f(t, y0);
            if (this->iteration == 3) this->fm1 = f(t, y0);

            ++this->iteration;
        }
        // After that we switch to Adams4
        else {
            this->fm4 = this->fm3;
            this->fm3 = this->fm2;
            this->fm2 = this->fm1;
            this->fm1 = f(t, y0);

            y0 += this->tau / 24. * (55. * this->fm1 - 59. * this->fm2 + 37. * this->fm3 - 9. * this->fm4);
            t += this->tau;
        }
    }

private:
    Uint      iteration = 0;
    Vector<N> fm1, fm2, fm3, fm4;
};

// RK4RE
// > 4-th order Runge-Kutta method with Richardson Extrapolation
// > Explicit, adaptive, O(tau^5)
template <Extent N = dynamic_size>
struct RK4RE : AdaptiveBase<N> {

    template <class Func>
    void operator()(Func&& f, Scalar& t, Vector<N>& y0) {
        
        Vector<N> w, y2;
        
        while (true) {
            // One double-step
            w = butcher::rk4::step(f, t, y0, 2 * this->tau);
            
            // Two regular steps
            y2 = butcher::rk4::step(f, t, y0, this->tau);
            y2 = butcher::rk4::step(f, t, y2, this->tau);

            // Error estimate
            Scalar err = 0; // = 1 / (2^p - 1) * max{...}
            for (Idx i = 0; i < y2.size(); ++i) err = std::max(err, std::abs(y2[i] - w[i]) / std::abs(y2[i]));
            err *= 1. / (16. - 1.);
            
            // Step correction
            // tau *= clamp( fact * (tol / err)^[1 / (p-1)], factmin, factmax )
            const Scalar tau_growth_factor = this->fact * std::pow(this->tolerance / err, 1. / (4. - 1.));
            this->tau *= std::clamp(tau_growth_factor, this->factmin, this->factmax);
            this->tau = std::clamp(this->tau, this->tau_min, this->tau_max);
            
            // Record steps
            if (err >= this->tolerance) {
                ++this->steps_discarded;
                continue;
            } else {
                ++this->steps_taken;
                break;
            }
            
        }
        
        y0 = y2 + (y2 - w) / (2. * 4 - 1); // (p + 1)-order approximation using a theorem
        t += 2 * this->tau;
    }
};

// DOPRI45
// > 4(5) Dormand-Prince (based on embedded 4/5-th order Runge-Kutta steps)
// > Explicit, adaptive, O(tau^5)
template <Extent N = dynamic_size>
struct DOPRI45 : AdaptiveBase<N> {

    template <class Func>
    void operator()(Func&& f, Scalar& t, Vector<N>& y0) {

        Vector<N> y, y_hat;

        while (true) {
            // Embedded step
            std::tie(y, y_hat) = butcher::dopri45::embedded_step(f, t, y0, this->tau);

            // Error estimate
            Scalar err = 0; // = 1 / (2^p - 1) * max{...}
            for (Idx i = 0; i < y_hat.size(); ++i) err = std::max(err, std::abs(y_hat[i] - y[i]) / std::abs(y_hat[i]));
            err *= 1. / (32. - 1.);
            
            // Step correction
            // tau *= clamp( fact * (tol / err)^[1 / (p-1)], factmin, factmax )
            const Scalar tau_growth_factor = this->fact * std::pow(this->tolerance / err, 1. / (5. - 1.));
            this->tau *= std::clamp(tau_growth_factor, this->factmin, this->factmax);
            this->tau = std::clamp(this->tau, this->tau_min, this->tau_max);
            
            // Record steps
            if (err >= this->tolerance) {
                ++this->steps_discarded;
                continue;
            } else {
                ++this->steps_taken;
                break;
            }
            
        };

        y0 = y_hat;
        t += this->tau;
    }
};

} // namespace gse::ode::integrators

// ==================
// --- ODE Solver ---
// ==================

namespace gse::ode {

// Type-erased functions
using RHS        = std::function<Vector<>(Scalar, Vector<>)>;
using Integrator = std::function<void(RHS f, Scalar& t, Vector<>& y0)>;
using Callback   = std::function<void(Scalar t, Vector<> y0, Integrator f)>;

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
          _require_invocable<Integrator, std::decay_t<Func>, Scalar&, Vector<N>&>   = true  //

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
        integrator(f, t, y0);
        t_since_callback += t - t_prev;

        if (verify) test_solution_for_divergence();
        handle_callback();
    }
}

} // namespace gse::ode
