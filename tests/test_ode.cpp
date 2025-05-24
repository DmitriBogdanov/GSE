// _______________ TEST FRAMEWORK & MODULE  _______________

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "thirdparty/doctest.h"

#include "test.hpp"

#include "GSE/core.hpp"
#include "GSE/ode.hpp"

// _______________________ INCLUDES _______________________

#include <cmath> // sin(), cols()

// ____________________ DEVELOPER DOCS ____________________

// Sanity tests for nonlinear solvers.
//
// Don't necessarily confirm all theoretical properties, but will catch blatant implementation errors.

// ____________________ IMPLEMENTATION ____________________

using Scalar = double;
using Vector = gse::Vector<Scalar, 2>;

const auto grid = gse::init::linspace<Scalar>(500, -100, 100); // 1D grid at which we test the derivative

TEST_CASE_TEMPLATE("Identity linear system", Method,         //
                   gse::ode::method::Euler<Scalar>,          //
                   gse::ode::method::RK4<Scalar>,            //
                   gse::ode::method::ImplicitEuler<Scalar>,  //
                   gse::ode::method::SymplecticEuler<Scalar> //
) {
    // Harmonic oscillator system:
    // { x' = v
    // { v' = - k/m x
    // { x(0) = x0
    // { v(0) = 0
    // with analytical solution x(t) = x(0) sin(sqrt(k/m)*t)
    //                          v(t) = x'(t)
    constexpr Scalar k = 2;
    constexpr Scalar m = 7;
    constexpr auto   f = [](Scalar, const Vector& u) -> Vector { return {u[1], -k / m * u[0]}; };

    const Vector x0 = {1, 0};
    const Scalar t0 = 0;
    const Scalar t1 = 10;

    const auto sol = [&](Scalar t) -> Vector {
        return {x0[0] * std::cos(std::sqrt(k / m) * t), -x0[0] * std::sqrt(k / m) * std::sin(std::sqrt(k / m) * t)};
    };

    const Vector solution_numerical  = gse::ode::solve(f, x0, t0, t1, Method{});
    const Vector solution_analytical = sol(t1);

    std::cout << "Numeric    -> " << solution_numerical << "\n";
    std::cout << "Analytical -> " << solution_analytical << "\n";

    REQUIRE(relative_error(solution_numerical[0], solution_analytical[0]) < 1e-2);
}