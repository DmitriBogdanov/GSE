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

// --- Indexation ---
// ------------------

using Idx    = Eigen::Index;             // ptrdiff_t
using Extent = decltype(Eigen::Dynamic); // int
using Uint   = unsigned int;

constexpr Extent dynamic_size = Eigen::Dynamic;

// --- Vector types ---
// --------------------

#ifdef GSE_USE_SCALAR_TYPE // controls library numeric precision
using Scalar = GSE_USE_SCALAR_TYPE; 
#else
using Scalar = double; 
#endif

// Motivation:
// We could have scalar type as a template argument everywhere, but realistically 99% of use
// cases want double precision. Single-precision is mainly useful for integrating on GPUs, which
// isn't supported by the library. Arbitrary precision is more of a novelty and shouldn't take
// priority over common use case convenience, if someone really wants to use arbitrary precision
// they can define a policy.

template <Extent rows = dynamic_size>
using Vector = Eigen::Vector<Scalar, rows>;

template <Extent rows = dynamic_size, Extent cols = dynamic_size>
using Matrix = Eigen::Matrix<Scalar, rows, cols>;

} // namespace gse::impl
