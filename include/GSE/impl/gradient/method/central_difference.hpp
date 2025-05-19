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

// Numerical gradient evaluation method. Uses central finite difference.
//    Error: O(h^2)
//    Cost:  2N 'f(x)' evaluations (~ 2N non-linear coef evaluations)

// ____________________ IMPLEMENTATION ____________________

namespace gse::impl::gradient::method {

struct CentralDifference {
    
    template <class T, Extent N, class Func, require_multivariate_function<T, N, Func> = true>
    Matrix<T, N, N> operator()(Func&& f, const Vector<T, N>& x) {
        constexpr T h       = math::cbrt(std::numeric_limits<T>::epsilon());
        constexpr T inv_2_h = 1 / (2 * h);
        // for central differences optimal step size is a cube root of machine epsilon,
        // see https://en.wikipedia.org/wiki/Numerical_differentiation

        Vector<T, N> x_plus_dxi = x;
        Vector<T, N> grad       = init::zero<T, N>(x.rows());

        for (Idx i = 0; i < x.rows(); ++i) {
            // grad(i) = (f(x + dxi) - f(x - dxi)) / 2h
            // rewritten to avoid temporary vectors
            x_plus_dxi[i] += h;
            grad[i] += f();
            x_plus_dxi[i] = x[i];

            x_plus_dxi[i] -= h;
            grad[i] -= f();
            x_plus_dxi[i] = x[i];

            grad[i] *= inv_2_h;
        }

        return grad;
    }
};

} // namespace gse::impl::gradient::method
