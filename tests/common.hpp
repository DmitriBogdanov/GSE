// __________________________________ CONTENTS ___________________________________
//
//    Common utils / includes / namespaces used for testing.
//    Reduces test boilerplate, should not be included anywhere else.
// _______________________________________________________________________________

#pragma once

// ___________________ TEST FRAMEWORK  ____________________

#define DOCTEST_CONFIG_VOID_CAST_EXPRESSIONS // makes 'CHECK_THROWS()' not give warning for discarding [[nodiscard]]
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN   // automatically creates 'main()' that runs tests
#include "doctest.h"

// _______________________ INCLUDES _______________________

#include "GSE/core.hpp" // gse::Extent, gse::Matrix<>

// ____________________ IMPLEMENTATION ____________________

template <class T, gse::impl::require_floating_point<T> = true>
[[nodiscard]] T relative_error(T approx, T precise) {
    constexpr T eps = 1e-6; // prevents division by zero when precise value is 0
    return std::abs(approx - precise) / (eps + std::abs(precise));
}

template <class T, gse::Extent N, gse::Extent M>
[[nodiscard]] T relative_error(const gse::Matrix<T, N, M>& approx, const gse::Matrix<T, N, M>& precise) {
    assert(approx.rows() == precise.rows());
    assert(approx.cols() == precise.cols());
    
    constexpr T eps = 1e-6; // prevents division by zero when precise value is 0
    return (approx - precise).norm() / (eps + (precise).norm());
}
