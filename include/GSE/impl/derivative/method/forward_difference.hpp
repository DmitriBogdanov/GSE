// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "../../core/traits.hpp"
#include "./base.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Numerical derivative evaluation method. Uses forward finite difference.
//    Error: O(h)
//    Cost:  2 'f(x)' evaluations (~ 2 non-linear coef evaluations)

// ____________________ IMPLEMENTATION ____________________

namespace gse::impl::derivative::method {

template <class T>
struct ForwardDifference : base::DirectionalScheme<T> {

    template <class Func, require_scalar_function<T, Func> = true>
    T operator()(Func&& f, T x) {
        const T h = this->diff_step;
        return (f(x + h) - f(x)) / h;
    }
};

} // namespace gse::impl::derivative::method
