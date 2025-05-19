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
#include "../../core/types.hpp"
#include "../../jacobian/solver.hpp"
#include "../../linear/solver.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Iteration of nonlinear root finding method. Uses Newton's algorithm.
//    Convergence: 2nd order
//    Cost:        Jacobian evaluation + linear system solution
//
// The naive code would be:
//    > x = x0 - J.inverse() * f(x0); // Newton's method iteration
// however
//    x = x0 - J(x0)^-1 * f(x0)
//    => (x - x0) = -J(x0)^-1 * f(x0)
//    => J(x0) * (x - x0) = -F(x0)
// so no need to compute the inverse, we can just solve a linear system.

// ____________________ IMPLEMENTATION ____________________

namespace gse::impl::nonlinear::method {

template <class JacobianMethod = jacobian::defaults::jacobian_method,
          class LinearMethod   = linear::defaults::linear_method>
struct Newton {

    template <class T, Extent N, class Func, require_vector_function<T, N, Func> = true>
    Vector<T, N> operator()(Func&& f, Vector<T, N> x0) {
        Matrix<T, N, N> J = jacobian::solve(f, x0, JacobianMethod{});

        return x0 + linear::solve(J, (-f(x0)).eval(), LinearMethod{});
        // without 'eval()' Eigen gets a little confused with expression templates
    }
};

} // namespace gse::impl::nonlinear::method
