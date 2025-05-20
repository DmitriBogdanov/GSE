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

// Numerical jacobian evaluation method. Uses central finite difference.
//    Error: O(h^2)
//    Cost:  2N 'f(x)' evaluations (~ 2N^2 non-linear coef evaluations)

// ____________________ IMPLEMENTATION ____________________

namespace gse::impl::jacobian::method {

template <class T>
struct CentralDifference : base::CentralScheme<T> {

    template <Extent N, class Func, require_vector_function<T, N, Func> = true>
    Matrix<T, N, N> operator()(Func&& f, const Vector<T, N>& x) {
        const T h = this->diff_step;

        Vector<T, N>    x_plus_dxj = x;
        Matrix<T, N, N> J          = init::zero<T, N, N>(x.rows(), x.rows());

        for (Idx j = 0; j < J.cols(); ++j) {
            // Each pair of 'f' invocations gives us a single column of the jacobian

            // Naive formula using central numerical derivative would be:
            //    > dfj = (f(x + dxj) - f(x - dxj)) * two_eps_inv;
            //    > J.col(j) = dfj;
            // where 'dxj' is a vector with '0' everywhere and 'half_diff_eps' on index 'j',
            // we rearrange things a bit to reduce allocations

            x_plus_dxj[j] += h;
            J.col(j) += f(x_plus_dxj);
            x_plus_dxj[j] = x[j];

            x_plus_dxj[j] -= h;
            J.col(j) -= f(x_plus_dxj);
            x_plus_dxj[j] = x[j];

            J.col(j) /= (2 * h);
        }

        return J;
    }
};

} // namespace gse::impl::jacobian::method
