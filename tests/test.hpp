#pragma once

// _______________________ INCLUDES _______________________

#include "GSE/core.hpp"

// ____________________ DEVELOPER DOCS ____________________

// This is a common include for all tests,
// it exists purely to reduce boilerplate and shouldn't be included anywhere else.

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
