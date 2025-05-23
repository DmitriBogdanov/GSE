// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "../../core/init.hpp"
#include "../../core/traits.hpp"
#include "../../core/types.hpp"
#include "./base.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Numerical gradient evaluation method. Uses central finite difference.
//    Error: O(h^2)
//    Cost:  2N 'f(x)' evaluations (~ 2N non-linear coef evaluations)

// ____________________ IMPLEMENTATION ____________________

namespace gse::impl::gradient::method {

template <class T>
struct CentralDifference : base::CentralScheme<T> {
    
    template <Extent N, class Func, require_multivariate_function<T, N, Func> = true>
    Vector<T, N> operator()(Func&& f, const Vector<T, N>& x) {
        const T h = this->diff_step;

        Vector<T, N> x_plus_dxi = x;
        Vector<T, N> grad       = init::zero<T, N>(x.rows());

        for (Idx i = 0; i < x.rows(); ++i) {
            // grad(i) = (f(x + dxi) - f(x - dxi)) / 2h
            // rewritten to avoid temporary vectors
            x_plus_dxi[i] += h;
            grad[i] += f(x_plus_dxi);
            x_plus_dxi[i] = x[i];

            x_plus_dxi[i] -= h;
            grad[i] -= f(x_plus_dxi);
            x_plus_dxi[i] = x[i];

            grad[i] /= (2 * h);
        }

        return grad;
    }
};

} // namespace gse::impl::gradient::method
