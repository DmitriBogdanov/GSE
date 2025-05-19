// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "../thirdparty/Eigen/Dense" // Matrix, Vector, Dynamic

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

// --- Vector type traits ---
// --------------------------

// Useful for compile time logic & type deduction

namespace meta {

template <class T>
using value_type = typename T::value_type;

template <class T>
constexpr Idx rows = T::RowsAtCompileTime;

template <class T>
constexpr Idx cols = T::ColsAtCompileTime;

}

} // namespace gse::impl
