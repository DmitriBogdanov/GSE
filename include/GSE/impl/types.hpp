// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "thirdparty/Eigen/Dense" // Matrix, Vector, Dynamic

// ____________________ DEVELOPER DOCS ____________________

// Core typedefs. The reason we use those 'gse::' typedefs instead of original templates
// is so we can have stylistically uniform API. Also provide a single customization point
// for numeric precision (if, for example, we want to compute with 'float' or 'long double').

// ____________________ IMPLEMENTATION ____________________

// ===================
// --- Basic Types ---
// ===================

namespace gse::impl {

using Idx    = Eigen::Index;             // ptrdiff_t
using Extent = decltype(Eigen::Dynamic); // int
using Uint   = unsigned int;

constexpr Extent dynamic_size = Eigen::Dynamic;

using Scalar = double; // controls library numeric precision

template <Extent rows = dynamic_size>
using Vector = Eigen::Vector<Scalar, rows>;

template <Extent rows = dynamic_size, Extent cols = dynamic_size>
using Matrix = Eigen::Matrix<Scalar, rows, cols>;

} // namespace gse::impl
