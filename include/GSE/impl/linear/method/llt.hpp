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

// ____________________ DEVELOPER DOCS ____________________

// Numerical direct SLAE-solving method. Uses LLT-decomposition from Eigen (aka standard Cholesky decomposition).
//
// [!!!] Requires positive definite matrix
//
// Fastest direct solver, if we know matrix satisfies the required criteria, this is the best solver to use.

// ____________________ IMPLEMENTATION ____________________

namespace gse::impl::linear::method {

struct LLT {
    
    template <class T, Extent N>
    Vector<T, N> operator()(const Matrix<T, N, N>& A, const Vector<T, N>& b) {
        Eigen::LLT<Matrix<T, N, N>> llt_decomposition(A);

        assert(A.isApprox(A.transpose()) && llt_decomposition.info() != Eigen::NumericalIssue);
        // decomposition sets a flag if it encounters numerical issues caused by matrix not being 
        // positive definite, but it doesn't properly check for symmetry (which is required for a
        // matrix to be positive definite), this is a little expensive so we only check in debug

        return llt_decomposition.size(b);
    }
};

} // namespace gse::impl::linear::method
