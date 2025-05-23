// _______________ TEST FRAMEWORK & MODULE  _______________

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "thirdparty/doctest.h"

#include "test.hpp"

#include "GSE/core.hpp"
#include "GSE/linear.hpp"

// _______________________ INCLUDES _______________________

// None

// ____________________ DEVELOPER DOCS ____________________

// Sanity tests for scalar derivative.
//
// Don't necessarily confirm all theoretical properties, but will catch blatant implementation errors.

// ____________________ IMPLEMENTATION ____________________

using Scalar = double;
using Vector = gse::Vector<Scalar>;
using Matrix = gse::Matrix<Scalar>;

const auto grid = gse::init::linspace<Scalar>(500, -100, 100); // 1D grid at which we test the derivative

TEST_CASE_TEMPLATE("Identity linear system", Method,    //
                   gse::linear::method::PartialPivotLU, //
                   gse::linear::method::FullPivotLU,    //
                   gse::linear::method::HouseholderQR,  //
                   gse::linear::method::LLT             // identity matrix is positive definite
) {
    // Identity system:
    // Ix = b
    // => x = b
    const gse::Idx size = 40;
    const Matrix   A    = gse::init::identity<Scalar>(size, size);
    const Vector   b    = gse::init::random<Scalar>(size);
    const Vector   x    = gse::linear::solve(A, b);

    REQUIRE(relative_error(b, x) < 1e-8);
}

TEST_CASE_TEMPLATE("Random linear system", Method,      //
                   gse::linear::method::PartialPivotLU, //
                   gse::linear::method::FullPivotLU,    //
                   gse::linear::method::HouseholderQR,  //
                   gse::linear::method::LLT             // identity matrix is positive definite
) {
    // Random system:
    // Ax = b
    const gse::Idx size = 40;
    const Matrix   A    = gse::init::random<Scalar>(size, size);
    const Vector   b    = gse::init::random<Scalar>(size);
    const Vector   x    = gse::linear::solve(A, b);

    REQUIRE(A.fullPivLu().isInvertible());
    // random matrices have a very low change of being singular, but we check to be sure

    REQUIRE(relative_error((A * x).eval(), b) < 1e-6);
}