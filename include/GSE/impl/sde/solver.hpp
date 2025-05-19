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

#include "../core/rng.hpp"
#include "../core/traits.hpp"
#include "../core/types.hpp"

#include "./method/base.hpp"
#include "./method/euler_maruyama.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Numerical solution of stochastic differential equation systems.
//
// Solver wraps iterative SDE integration method in a loop over 't'.
// Has several overloads depending on the presence of callbacks & verification requirements.
//
// A lot of logic more-or-less duplicates that of the ODE the solver, but there doesn't seem
// to be any nice way to generalize that without complicating things unreasonably.
//
// Function signatures might seem unwieldy due to all the template parameters and SFINAE, but
// on user side everything is just as simple as with all the other solvers.

// ____________________ IMPLEMENTATION ____________________

// ================
// --- Defaults ---
// ================

namespace gse::impl::sde::defaults {

template <class T>
using sde_method = method::EulerMaruyama<T>;

using generator = PRNG;

template <class T>
using normal_distribution = NormalDistribution<T>;

constexpr Uint number_of_callbacks = 100;

} // namespace gse::impl::sde::defaults

// ==============
// --- SFINAE ---
// ==============

namespace gse::impl::sde {

// SFINAE helpers to validate callable signatures.
// In this case this isn't just a good general practice, it is necessary
// to avoid the ambiguity between some template overloads of 'solve()'
template <class T, Extent N, class Callback, class Method>
using require_valid_callback = require_invocable<Callback, T, Vector<T, N>, Method>;

template <class T, Extent N, class FuncA, class FuncB, class Gen, class Dist, class Method>
using require_valid_method =
    require_invocable_r<std::pair<T, Vector<T, N>>, Method, FuncA, FuncB, Gen, Dist, T, Vector<T, N>>;

} // namespace gse::impl::sde

// ==============
// --- Solver ---
// ==============

namespace gse::impl::sde {

// Solve Cauchy's problem for an SDE system (written in Ito form):
//    { dx(t) = A(t, x) dt + B(t, x) dW,   A: {R, R^N} -> R^N, B: {R, R^N} -> R^N,
//    { x(0)  = x0,                        t0 <= t <= t1.
//
// here:
//    'A'      - deterministic part of system RHS,
//    'B'      - stochastic    part of system RHS,
//    'x0'     - initial condition,
//    't0'     - time interval start,
//    't1'     - time interval end,
//    'method' - integration method,
//    'gen'    - underlying random number generator,
//    'dist'   - underlying normal distribution.
//
// Minimalistic overload with only integration & end result.
//
template <class T, Extent N, class FuncA, class FuncB, class Method = defaults::sde_method<T>,
          class Gen = defaults::generator, class Dist = defaults::normal_distribution<T>,
          require_time_vector_function<T, N, FuncA>                   = true, //
          require_time_vector_function<T, N, FuncB>                   = true, //
          require_valid_method<T, N, FuncA, FuncB, Gen, Dist, Method> = true  //
          >
Vector<T, N> solve(FuncA&&             A,                 //
                   FuncB&&             B,                 //
                   const Vector<T, N>& x0,                //
                   T                   t0,                //
                   T                   t1,                //
                   Method&&            method = Method{}, //
                   Gen&&               gen    = Gen{},    //
                   Dist&&              dist   = Dist{}    //
) {
    T            t = t0;
    Vector<T, N> x = x0;

    while (t < t1) std::tie(t, x) = method(A, B, gen, dist, t, std::move(x));

    return x;
}

// Solve Cauchy's problem for an SDE system (written in Ito form):
//    { dx(t) = A(t, x) dt + B(t, x) dW,   A: {R, R^N} -> R^N, B: {R, R^N} -> R^N,
//    { x(0)  = x0,                        t0 <= t <= t1.
//
// here:
//    'A'      - deterministic part of system RHS,
//    'B'      - stochastic    part of system RHS,
//    'x0'     - initial condition,
//    't0'     - time interval start,
//    't1'     - time interval end,
//    'method' - integration method,
//    'gen'    - underlying random number generator,
//    'dist'   - underlying normal distribution.
//
// Overload with callback & manual frequency.
//
template <class T, Extent N, class FuncA, class FuncB, class Callback, class Method = defaults::sde_method<T>,
          class Gen = defaults::generator, class Dist = defaults::normal_distribution<T>,
          require_time_vector_function<T, N, FuncA>                   = true, //
          require_time_vector_function<T, N, FuncB>                   = true, //
          require_valid_callback<T, N, Callback, Method>              = true, //
          require_valid_method<T, N, FuncA, FuncB, Gen, Dist, Method> = true  //
          >
Vector<T, N> solve(FuncA&&             A,                  //
                   FuncB&&             B,                  //
                   const Vector<T, N>& x0,                 //
                   T                   t0,                 //
                   T                   t1,                 //
                   Callback&&          callback,           //
                   T                   callback_frequency, //
                   Method&&            method = Method{},  //
                   Gen&&               gen    = Gen{},     //
                   Dist&&              dist   = Dist{}     //
) {
    T            t = t0;
    Vector<T, N> x = x0;

    T time_since_callback = 0;

    callback(t, x, method); // t0 always gets a callback

    while (t < t1) {
        std::tie(t, x) = method(A, B, gen, dist, t, std::move(x));

        time_since_callback += method.time_step; // we assume all methods have that

        if (time_since_callback >= callback_frequency) {
            time_since_callback -= callback_frequency;
            callback(t, x, method);
        }
    }

    return x;
}

// Solve Cauchy's problem for an ODE system:
//    { x'(t) = f(t, x),   f: {R, R^N} -> R^N,
//    { x(0)  = x0,        t0 <= t <= t1.
//
// here:
//    'f'              - system RHS,
//    'x0'             - initial condition,
//    't0'             - time interval start,
//    't1'             - time interval end,
//    'method'         - integration method.
//
// Overload with callback & automatic frequency.
//
template <class T, Extent N, class FuncA, class FuncB, class Callback, class Method = defaults::sde_method<T>,
          class Gen = defaults::generator, class Dist = defaults::normal_distribution<T>,
          require_time_vector_function<T, N, FuncA>                   = true, //
          require_time_vector_function<T, N, FuncB>                   = true, //
          require_valid_callback<T, N, Callback, Method>              = true, //
          require_valid_method<T, N, FuncA, FuncB, Gen, Dist, Method> = true  //
          >
Vector<T, N> solve(FuncA&&             A,                 //
                   FuncB&&             B,                 //
                   const Vector<T, N>& x0,                //
                   T                   t0,                //
                   T                   t1,                //
                   Callback&&          callback,          //
                   Method&&            method = Method{}, //
                   Gen&&               gen    = Gen{},    //
                   Dist&&              dist   = Dist{}    //
) {
    const T callback_frequency = (t1 - t0) / defaults::number_of_callbacks;

    return solve(std::forward<FuncA>(A), std::forward<FuncB>(B), x0, t0, t1, std::forward<Callback>(callback),
                 callback_frequency, std::forward<Method>(method), std::forward<Gen>(gen), std::forward<Dist>(dist));
}
// if C++ supported default function parameters depending on other parameters,
// we would just default set 'T callback_frequency = (t1 - t0) / default::number_of_callbacks',
// since that isn't a case we have to emulate such API though a separate overload

} // namespace gse::impl::sde
