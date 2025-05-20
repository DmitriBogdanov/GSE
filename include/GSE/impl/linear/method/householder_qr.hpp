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
#include "GSE/impl/thirdparty/Eigen/src/Core/util/Macros.h"
#include "GSE/impl/thirdparty/Eigen/src/QR/HouseholderQR.h"

// ____________________ DEVELOPER DOCS ____________________

// Numerical direct SLAE-solving method. Uses QR-decomposition from Eigen based on Householder reflections.
//
// Slower than partial pivot LU, but faster than full pivot LU.

// ____________________ IMPLEMENTATION ____________________

namespace gse::impl::linear::method {

struct HouseholderQR {
    
    template <class T, Extent N>
    Vector<T, N> operator()(const Matrix<T, N, N>& A, const Vector<T, N>& b) {
        return A.householderQr().solve(b);  
    }
};

} // namespace gse::impl::jacobian::method
