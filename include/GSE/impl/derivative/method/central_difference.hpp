// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include <limits> // numeric_limits<>::epsilon()

#include "../../core/init.hpp"
#include "../../core/math.hpp"
#include "../../core/traits.hpp"
#include "../../core/types.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Numerical derivative evaluation method. Uses central finite difference.
//    Error: O(h^2)
//    Cost:  2 'f(x)' evaluations (~ 2 non-linear coef evaluations)

// ____________________ IMPLEMENTATION ____________________

namespace gse::impl::derivative::method {

struct CentralDifference {
    
    template <class T, Extent N, class Func, require_scalar_function<T, N, Func> = true>
    T operator()(Func&& f, T x) {
        constexpr T h       = math::cbrt(std::numeric_limits<T>::epsilon());
        constexpr T inv_2_h = 1 / (2 * h);
        // for central differences optimal step size is a cube root of machine epsilon,
        // see https://en.wikipedia.org/wiki/Numerical_differentiation

        return (f(x + h) - f(x - h)) * inv_2_h;
    }
};

} // namespace gse::impl::gradient::method
