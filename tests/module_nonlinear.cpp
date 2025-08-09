#include "common.hpp"

#include "GSE/nonlinear.hpp"

// _______________________ INCLUDES _______________________

#include <cmath> // sin(), cols()

// ____________________ IMPLEMENTATION ____________________

using Scalar = double;
using Vector = gse::Vector<Scalar, 2>;

const auto grid = gse::init::linspace<Scalar>(500, -100, 100); // 1D grid at which we test the derivative

TEST_CASE_TEMPLATE("Single-root nonlinear system", Method, //
                   gse::nonlinear::method::Newton<Scalar>  //
) {
    // Nonlinear system:
    // { exp(x0 - 2) = x1
    // { x1^2 = x0
    // => x = 0.019026
    //    y = 0.137935
    constexpr auto f = [](Vector x) { return Vector{std::exp(x[0] - 2) - x[1], x[1] * x[1] - x[0]}; };

    const Vector solution_numerical = gse::nonlinear::solve(f, Vector{0, 0});

    const Vector solution_analytical = {0.019026, 0.137935};

    REQUIRE(relative_error(solution_numerical, solution_analytical) < 1e-5);
}