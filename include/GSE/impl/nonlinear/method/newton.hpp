// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "../../core/types.hpp"
#include "../../jacobian/solver.hpp"
#include "../../linear/solver.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Iteration of nonlinear root finding method. Uses Newton's algorithm.
//    Convergence: 2nd order
//    Cost:        Jacobian evaluation + linear system solution
//
// The naive code would be:
//    > y = y = y0 - J.inverse() * F(y0); // Newton's method iteration
// however
//    y = y0 - J(y0)^-1 * F(y0)
//    => (y - y0) = -J(y0)^-1 * F(y0)
//    => J(y0) * (y - y0) = -F(y0)
// so no need to compute the inverse, we can just solve a linear system.

// ____________________ IMPLEMENTATION ____________________

namespace gse::impl::nonlinear::method {

template <class JacobianMethod = jacobian::defaults::jacobian_method,
          class LinearMethod   = linear::defaults::linear_method>
struct Newton {

    template <class T, Extent N, class Func>
    Vector<T, N> operator()(Func&& f, Vector<T, N> x0) {
        Matrix<T, N, N> J = jacobian::solve(f, x0, JacobianMethod{});
        Vector<T, N>    x = x0 + linear::solve(J, (-f(x0)).eval(), LinearMethod{});
        // without 'eval()' Eigen gets a little confused with expression templates
        
        return x;
    }
};

} // namespace gse::impl::nonlinear::method
