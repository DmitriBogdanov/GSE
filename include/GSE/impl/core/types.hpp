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

#include "../thirdparty/Eigen/Dense" // Matrix, Vector, Dynamic, ...

#ifdef GSE_IMPL_SILENCE_GCC_11_13_FALSE_POSITIVE
#pragma GCC diagnostic pop
#endif

// ____________________ DEVELOPER DOCS ____________________

// Core typedefs. The reason we use those 'gse::' typedefs instead of
// original templates is so we can have stylistically uniform API.

// ____________________ IMPLEMENTATION ____________________

// ===================
// --- Basic Types ---
// ===================

namespace gse::impl {

// --- Indexation ---
// ------------------

using Idx    = Eigen::Index;             // ptrdiff_t
using Extent = decltype(Eigen::Dynamic); // int
using Uint   = unsigned int;

constexpr Extent dynamic = Eigen::Dynamic;

// --- Vector types ---
// --------------------

template <class T, Extent N = dynamic>
using Vector = Eigen::Vector<T, N>;

template <class T, Extent N = dynamic, Extent M = dynamic>
using Matrix = Eigen::Matrix<T, N, M>;

// --- Base types ---
// ------------------

template <class Derived>
using MatrixBase = Eigen::MatrixBase<Derived>; // dense matrices (support matrix operations)

template <class Derived>
using ArrayBase = Eigen::ArrayBase<Derived>; // dense arrays (support element-wise operations)

template <class Derived>
using DenseBase = Eigen::DenseBase<Derived>; // dense arrays + matrices (support both matrix & element-wise operations)

} // namespace gse::impl
