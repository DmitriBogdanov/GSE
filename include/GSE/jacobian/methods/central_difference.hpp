// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "GSE/core/initialize.hpp"
#include "GSE/jacobian/method.hpp"

// ____________________ DEVELOPER DOCS ____________________

// None

// ____________________ IMPLEMENTATION ____________________

namespace gse::jacobian::methods {

// [ LSP documentation ]
//
// 2nd order numerical differentiation method.
//
// A good default for most problems.
//
template <scalar T>
struct CentralDifference : impl::CentralScheme<T> {

    template <Extent N, vector_function<T, N> Func>
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

static_assert(method_function<CentralDifference<double>, double, dynamic>);

} // namespace gse::jacobian::methods