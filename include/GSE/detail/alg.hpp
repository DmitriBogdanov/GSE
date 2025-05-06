// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "core.hpp"
#include <iostream> // TEMP:
// ____________________ DEVELOPER DOCS ____________________

// TODO: DOCS

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

// TEMP:
// Computes Jacobian of F(y) at y = X using numeric differentiation
template <Extent N = dynamic_size, class Func>
inline Matrix<N> _legacy_jacobian(Func&& F, Vector<N> X) {
    constexpr Scalar diff_eps         = 2e-8;
    constexpr Scalar diff_eps_inverse = 1. / diff_eps;

    const auto size = X.size();

    Matrix<N> J = Matrix<N>::Zero(size, size);

    Vector<N> dXj = Vector<N>::Zero(size);

    for (int j = 0; j < N; ++j) {
        // Differentiate F with respect to x_j
        dXj(j) += diff_eps;
        const auto dFj = (F(X + 0.5 * dXj) - F(X - 0.5 * dXj)) * diff_eps_inverse;
        dXj(j)         = 0.;

        // As a result we get an entire column of jacobian
        J.col(j) = dFj;
    }

    return J;
}
// TEMP:

} // namespace gse::alg

// ========================
// --- Algebraic Solver ---
// ========================

namespace gse::alg {

// TEMP:
// Solve system of algebraic equations F(y) = 0 using y0 as an initial guess
template <Extent N, class Func>
inline Vector<N> _legacy_solve(Func&& F, Vector<N> y0, Scalar precision, Uint maxIterations = 100) {
    Vector<N> y = y0;

    // Fill approximations
    for (Uint iterations = 0; iterations < maxIterations; ++iterations) {
        // The naive code would be:
        //    > y = y0 - jacobian(F, y0).inverse() * F(y0); // Newthon's method iteration
        // however
        //    y = y0 - J_f(y0)^-1 * f(y0)
        //    => (y - y0) = -J_f(y0)^-1 * f(y0)
        //    => J_f(y0) * (y - y0) = -f(y0)
        // so no need to compute the inverse, we can just solve a SLAE
        y += jacobian(F, y0).partialPivLu().solve(F(y0));

        if ((y - y0).norm() < precision) break;

        y0 = y;
    }

    return y;
}
// TEMP:

// Solve system of algebraic equations F(y) = 0 using y0 as an initial guess
template <Extent N, class Func>
inline Vector<N> solve(Func&& F, Vector<N> y0, Scalar precision, Uint max_iterations = 100) {
    Vector<N> y = y0;

    Matrix<N, N>              J = init::zero<N>(N, N);
    JacobianDifferentiator<N> differentiator;

    // Fill approximations
    for (Uint iterations = 0; iterations < max_iterations; ++iterations) {
        // The naive code would be:
        //    > y = y = y0 - J.inverse() * F(y0); // Newthon's method iteration
        // however
        //    y = y0 - J(y0)^-1 * F(y0)
        //    => (y - y0) = -J(y0)^-1 * F(y0)
        //    => J(y0) * (y - y0) = -F(y0)
        // so no need to compute the inverse, we can just solve a SLAE
        differentiator(J, F, y0);
        y = y0 + J.fullPivLu().solve(-F(y0));

        if ((y - y0).norm() < precision) break;

        y0 = y;
    }

    return y;
}

} // namespace gse::alg
