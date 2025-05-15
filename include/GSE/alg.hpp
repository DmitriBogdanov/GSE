// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "core.hpp" // ...

// ____________________ DEVELOPER DOCS ____________________

// Jacobian computation. Algebraic system solver.

// ____________________ IMPLEMENTATION ____________________

// =================================
// --- Numerical differentiation ---
// =================================

namespace gse::alg {

constexpr Scalar diff_eps         = 2e-8;
constexpr Scalar half_diff_eps    = diff_eps / 2.;
constexpr Scalar diff_eps_inverse = 1. / diff_eps;

template <Extent N = dynamic_size>
struct JacobianDifferentiator {
    // - Differentiator -
    template <class Func>
    void operator()(Matrix<N, N>& J, Func&& F, const Vector<N>& X) {
        Vector<N> tmp = X;

        for (Idx j = 0; j < J.cols(); ++j) {
            // Naive formula using central numerical derivative would be:
            //    > dFj = (F(X + 0.5 * dXj) - F(X - 0.5 * dXj)) * diff_eps_inverse;
            //      (where 'dXj' is a vector with '0' everywhere and 'half_diff_eps' on index 'j')
            //    > J.col(j) = dFj;
            // we rearrange things a bit to reduce allocations
            tmp[j] += half_diff_eps;
            J.col(j) += F(tmp);
            tmp[j] = X[j];

            tmp[j] -= half_diff_eps;
            J.col(j) -= F(tmp);
            tmp[j] = X[j];

            J.col(j) *= diff_eps_inverse;
        }
    }
};

} // namespace gse::alg

// ========================
// --- Algebraic Solver ---
// ========================

namespace gse::alg {

// Solve system of algebraic equations F(y) = 0 using y0 as an initial guess
template <Extent N, class Func>
inline Vector<N> solve(Func&& F, Vector<N> y0, Scalar precision, Uint max_iterations = 100) {
    Vector<N> y = y0;

    Matrix<N, N>              J = init::zero<N>(N, N);
    JacobianDifferentiator<N> differentiator;

    // Fill approximations
    for (Uint iterations = 0; iterations < max_iterations; ++iterations) {
        // The naive code would be:
        //    > y = y = y0 - J.inverse() * F(y0); // Newton's method iteration
        // however
        //    y = y0 - J(y0)^-1 * F(y0)
        //    => (y - y0) = -J(y0)^-1 * F(y0)
        //    => J(y0) * (y - y0) = -F(y0)
        // so no need to compute the inverse, we can just solve a linear system
        differentiator(J, F, y0);
        y = y0 + J.fullPivLu().solve(-F(y0)); // slow, but we want as much precision as possible

        if ((y - y0).norm() < precision) break;

        y0 = y;
    }

    return y;
}

} // namespace gse::alg
