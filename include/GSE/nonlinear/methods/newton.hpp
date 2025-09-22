// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "GSE/jacobian/solver.hpp"
#include "GSE/linear/solver.hpp"
#include "GSE/nonlinear/method.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Iteration of Newton's root finding method:
//    Convergence: 2nd order
//    Cost:        Jacobian evaluation + linear system solution
//
// The naive code would be:
//    > x = x0 - J.inverse() * f(x0); // Newton's method iteration
// however
//    x = x0 - J(x0)^-1 * f(x0)
//    => (x - x0) = -J(x0)^-1 * f(x0)
//    => J(x0) * (x - x0) = -f(x0)
// so no need to compute the inverse, we can just solve a linear system.

// ____________________ IMPLEMENTATION ____________________

namespace gse::nonlinear::methods {

// [ LSP documentation ]
//
// 2nd order method for solving systems of nonlinear equations.
//
// A good default for most problems.
//
template <scalar T, Extent N,                                                             //
          jacobian::method_function<T, N> JacobianMethod = jacobian::defaults::method<T>, //
          linear::method_function<T, N>   LinearMethod   = linear::defaults::method       //
          >
struct Newton : impl::IterativeScheme<T> {

    JacobianMethod jacobian_method;
    LinearMethod   linear_method;

    template <vector_function<T, N> Func>
    Vector<T, N> operator()(Func&& f, Vector<T, N> x0) {
        Matrix<T, N, N> J   = jacobian::solve(f, x0, this->jacobian_method);
        Vector<T, N>    rhs = -f(x0);

        return x0 + linear::solve(J, rhs, this->linear_method);
    }
};

static_assert(method_function<Newton<double, dynamic>, double, dynamic>);

} // namespace gse::nonlinear::methods
