// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

// Silences false positive warnings on GCC 11-13 about uninitialized variables in 'TriangularMatrixVector.h'.
// In this codebase the warning gets triggered by 'A.householderQr().solve(b)' in linear methods,
// see https://gitlab.com/libeigen/eigen/-/issues/2787
#if defined(__GNUC__) && !defined(__clang__) && (__GNUC__ < 13) && (__GNUC__ > 11)
#define GSE_IMPL_SILENCE_GCC_11_13_FALSE_POSITIVE
#endif

#ifdef GSE_IMPL_SILENCE_GCC_11_13_FALSE_POSITIVE
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif

#include "Eigen/Dense" // Matrix, Vector, Dynamic, ...

#ifdef GSE_IMPL_SILENCE_GCC_11_13_FALSE_POSITIVE
#pragma GCC diagnostic pop
#endif



// ____________________ DEVELOPER DOCS ____________________

// Minimal include for the core matrix types.

// ____________________ IMPLEMENTATION ____________________

namespace gse {

// ==================
// --- Indexation ---
// ==================

// [ LSP documentation ]
//
// Index type used by GSE matrices.
//
using Idx = Eigen::Index; // ptrdiff_t

// [ LSP documentation ]
//
// Extent type used by GSE matrices.
//
using Extent = decltype(Eigen::Dynamic); // int

// [ LSP documentation ]
//
// Unsigned type used for positive counters.
//
using Uint = unsigned int;

// [ LSP documentation ]
//
// Value specifying that corresponding matrix extent should be dynamic.
//
constexpr Extent dynamic = Eigen::Dynamic;

// ====================
// --- Vector types ---
// ====================

// [ LSP documentation ]
//
// Row-major column vector. Templated on:
//    `T` - scalar type,
//    `N` - rows extent (static or dynamic).
//
template <class T = double, Extent N = dynamic>
using Vector = Eigen::Vector<T, N>;

// [ LSP documentation ]
//
// Row-major matrix. Templated on:
//    `T` - scalar type,
//    `M` - rows extent (static or dynamic).
//    `N` - columns extent (static or dynamic).
//
template <class T= double, Extent M = dynamic, Extent N = dynamic>
using Matrix = Eigen::Matrix<T, N, M>;

} // namespace gse
