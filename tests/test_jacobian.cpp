// _______________ TEST FRAMEWORK & MODULE  _______________

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "thirdparty/doctest.h"

#include "test.hpp"

#include "GSE/core.hpp"
#include "GSE/jacobian.hpp"

// _______________________ INCLUDES _______________________

#include <cmath> // sin(), cols()

// ____________________ DEVELOPER DOCS ____________________

// Sanity tests for vector function jacobian.
//
// Don't necessarily confirm all theoretical properties, but will catch blatant implementation errors.

// ____________________ IMPLEMENTATION ____________________

using Scalar = double;
using Vector = gse::Vector<Scalar, 2>;
using Matrix = gse::Matrix<Scalar, 2, 2>;

const auto grid_x0 = gse::init::linspace<Scalar>(40, -100, 100); // 2D grid at which we test the jacobian
const auto grid_x1 = gse::init::linspace<Scalar>(40, -100, 100);

TEST_CASE_TEMPLATE("Trigonometric jacobian", Method,                //
                   gse::jacobian::method::CentralDifference<Scalar> //
) {
    // f(x) = {  sin(x0)sin(x1)  cos(x0)cos(x1) }
    //
    // J(x) = [  cos(x0)sin(x1)  sin(x0)cos(x1) ]
    //        [ -sin(x0)cos(x1) -cos(x0)sin(x1) ]
    constexpr auto f = [](Vector x) {
        return Vector{std::sin(x[0]) * std::sin(x[1]), std::cos(x[0]) * std::cos(x[1])};
    };
    constexpr auto J = [](Vector x) {
        return Matrix{
            { std::cos(x[0]) * std::sin(x[1]),  std::sin(x[0]) * std::cos(x[1])},
            {-std::sin(x[0]) * std::cos(x[1]), -std::cos(x[0]) * std::sin(x[1])}
        };
    };

    for (const auto& x0 : grid_x0) {
        for (auto x1 : grid_x1) {
            const Vector x                   = {x0, x1};
            const Matrix numerical_jacobian  = gse::jacobian::solve(f, x, Method{});
            const Matrix analytical_jacobian = J(x);

            REQUIRE(relative_error(numerical_jacobian, analytical_jacobian) < 1e-5);
        }
    }
}