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

#include "../core/traits.hpp"
#include "../core/types.hpp"

#include "./method/base.hpp"
#include "./method/euler.hpp"
#include "./method/implicit_euler.hpp"
#include "./method/rk4.hpp"
#include "./method/symplectic_euler.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Numerical solution of ordinary differential equation systems.
//
// Solver wraps iterative ODE integration method in a loop over 't'.
// Has several overloads depending on the presence of callbacks & verification requirements.

// ____________________ IMPLEMENTATION ____________________

// ================
// --- Defaults ---
// ================

namespace gse::impl::ode::defaults {

template <class T>
using ode_method = method::RK4<T>;

constexpr Uint number_of_callbacks = 100;

} // namespace gse::impl::ode::defaults

// ==============
// --- SFINAE ---
// ==============

namespace gse::impl::ode {

// SFINAE helpers to validate callable signatures.
// In this case this isn't just a good general practice, it is necessary
// to avoid the ambiguity between some template overloads of 'solve()'
template <class T, Extent N, class Func>
using require_valid_func = require_invocable_r<Vector<T, N>, Func, T, Vector<T, N>>;

template <class T, Extent N, class Callback, class Method>
using require_valid_callback = require_invocable<Callback, T, Vector<T, N>, Method>;

template <class T, Extent N, class Func, class Method>
using require_valid_method = require_invocable_r<std::pair<T, Vector<T, N>>, Method, Func, T, Vector<T, N>>;

} // namespace gse::impl::ode

// ==============
// --- Solver ---
// ==============

namespace gse::impl::ode {

// Solve Cauchy's problem for an ODE system:
//    { x'(t) = f(t, x),   f: {R, R^N} -> R^N,
//    { x(0)  = x0,        t0 <= t <= t1.
//
// here:
//    'f'      - system RHS,
//    'x0'     - initial condition,
//    't0'     - time interval start,
//    't1'     - time interval end,
//    'method' - integration method.
//
// Minimalistic overload with only integration & end result.
//
template <class T, Extent N, class Func, class Method = defaults::ode_method<T>,
          require_valid_func<T, N, Func>           = true, //
          require_valid_method<T, N, Func, Method> = true  //
          >
Vector<T, N> solve(Func&&              f,                //
                   const Vector<T, N>& x0,               //
                   T                   t0,               //
                   T                   t1,               //
                   Method&&            method = Method{} //
) {
    T            t = t0;
    Vector<T, N> x = x0;

    while (t < t1) std::tie(t, x) = method(f, t, std::move(x));

    return x;
}

// Solve Cauchy's problem for an ODE system:
//    { x'(t) = f(t, x),   f: {R, R^N} -> R^N,
//    { x(0)  = x0,        t0 <= t <= t1.
//
// here:
//    'f'      - system RHS,
//    'x0'     - initial condition,
//    't0'     - time interval start,
//    't1'     - time interval end,
//    'method' - integration method.
//
// Overload with callback & manual frequency.
//
template <class T, Extent N, class Func, class Callback, class Method = defaults::ode_method<T>,
          require_valid_func<T, N, Func>                 = true, //
          require_valid_callback<T, N, Callback, Method> = true, //
          require_valid_method<T, N, Func, Method>       = true  //
          >
Vector<T, N> solve(Func&&              f,                  //
                   const Vector<T, N>& x0,                 //
                   T                   t0,                 //
                   T                   t1,                 //
                   Callback&&          callback,           //
                   T                   callback_frequency, //
                   Method&&            method = Method{}   //
) {
    T            t = t0;
    Vector<T, N> x = x0;

    T time_since_callback = 0;

    callback(t, x, method); // t0 always gets a callback

    while (t < t1) {
        std::tie(t, x) = method(f, t, std::move(x));

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
//    'f'      - system RHS,
//    'x0'     - initial condition,
//    't0'     - time interval start,
//    't1'     - time interval end,
//    'method' - integration method.
//
// Overload with callback & automatic frequency.
//
template <class T, Extent N, class Func, class Callback, class Method = defaults::ode_method<T>,
          require_valid_func<T, N, Func>                 = true, //
          require_valid_callback<T, N, Callback, Method> = true, //
          require_valid_method<T, N, Func, Method>       = true  //
          >
Vector<T, N> solve(Func&&              f,                //
                   const Vector<T, N>& x0,               //
                   T                   t0,               //
                   T                   t1,               //
                   Callback&&          callback,         //
                   Method&&            method = Method{} //
) {
    const T callback_frequency = (t1 - t0) / defaults::number_of_callbacks;

    return solve(std::forward<Func>(f), x0, t0, t1, std::forward<Callback>(callback), callback_frequency,
                 std::forward<Method>(method));
}
// if C++ supported default function parameters depending on other parameters,
// we would just default set 'T callback_frequency = (t1 - t0) / default::number_of_callbacks',
// since that isn't a case we have to emulate such API though a separate overload

} // namespace gse::impl::ode
