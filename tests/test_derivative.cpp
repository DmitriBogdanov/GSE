// _______________ TEST FRAMEWORK & MODULE  _______________

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "thirdparty/doctest.h"

#include "test.hpp"

#include "GSE/core.hpp"
#include "GSE/derivative.hpp"

// _______________________ INCLUDES _______________________

#include <cmath> // sin(), cols()

// ____________________ DEVELOPER DOCS ____________________

// Sanity tests for scalar derivative.
//
// Don't necessarily confirm all theoretical properties, but will catch blatant implementation errors.

// ____________________ IMPLEMENTATION ____________________

using Scalar = double;

const auto grid = gse::init::linspace<Scalar>(500, -10, 10); // 1D grid at which we test the derivative

TEST_CASE_TEMPLATE("Polynomial derivative", Method,                            //
                   gse::derivative::method::ForwardDifference<Scalar>,         //
                   gse::derivative::method::BackwardDifference<Scalar>,        //
                   gse::derivative::method::CentralDifference<Scalar>,         //
                   gse::derivative::method::FourPointCentralDifference<Scalar> //
) {
    // f(x)  = x^3 + x^2 + x
    //
    // f'(x) = 3x^2 + 2x + 1
    constexpr auto f  = [](Scalar x) { return x * x * x + x * x + x; };
    constexpr auto df = [](Scalar x) { return 3 * x * x + 2 * x + 1; };

    for (const auto& x : grid) {
        const Scalar numerical_derivative  = gse::derivative::solve(f, x, Method{});
        const Scalar analytical_derivative = df(x);

        REQUIRE(relative_error(numerical_derivative, analytical_derivative) < 1e-5);
    }
}

TEST_CASE_TEMPLATE("Trigonometric derivative", Method,                         //
                   gse::derivative::method::ForwardDifference<Scalar>,         //
                   gse::derivative::method::BackwardDifference<Scalar>,        //
                   gse::derivative::method::CentralDifference<Scalar>,         //
                   gse::derivative::method::FourPointCentralDifference<Scalar> //
) {
    // f(x)  = sin(x) + cos(x)
    //
    // f'(x) = cos(x) - sin(x)
    constexpr auto f  = [](Scalar x) { return std::sin(x) + std::cos(x); };
    constexpr auto df = [](Scalar x) { return std::cos(x) - std::sin(x); };

    for (const auto& x : grid) {
        const Scalar numerical_derivative  = gse::derivative::solve(f, x, Method{});
        const Scalar analytical_derivative = df(x);

        REQUIRE(relative_error(numerical_derivative, analytical_derivative) < 1e-5);
    }
}