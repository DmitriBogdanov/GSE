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

// ____________________ DEVELOPER DOCS ____________________

// Numerical jacobian evaluation method. Uses central finite difference.
//    Error: O(h^2)
//    Cost:  2N 'f(x)' evaluations

// ____________________ IMPLEMENTATION ____________________

namespace gse::impl::jacobian::method {

struct CentralDifference {
    template <class T, Extent N, class Func>
    Matrix<T, N, N> operator()(Func&& f, const Vector<T, N>& x) {
        constexpr T eps         = is_float_v<T> ? 0.00492157 : 6.05545e-6;
        constexpr T two_eps_inv = 1 / (2 * eps);
        // for central differences optimal step size is a cube root of machine epsilon,
        // see https://en.wikipedia.org/wiki/Numerical_differentiation

        Vector<T, N>    dxj = x;
        Matrix<T, N, N> J   = init::zero<T, N, N>(x.rows(), x.rows());

        for (Idx j = 0; j < J.cols(); ++j) {
            // Each pair of 'f' invocations gives us a single column of the jacobian

            // Naive formula using central numerical derivative would be:
            //    > dfj = (f(x + dxj) - f(x - dxj)) * two_eps_inv;
            //    > J.col(j) = dfj;
            // where 'dxj' is a vector with '0' everywhere and 'half_diff_eps' on index 'j',
            // we rearrange things a bit to reduce allocations

            dxj[j] += eps;
            J.col(j) += f(dxj);
            dxj[j] = x[j];

            dxj[j] -= eps;
            J.col(j) -= f(dxj);
            dxj[j] = x[j];

            J.col(j) *= two_eps_inv;
        }
        
        return J;
    }
};

} // namespace gse::impl::jacobian::method
