// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include <array>            // array<>
#include <initializer_list> // initializer_list<>
#include <vector>           // vector<>

#include "impl/format.hpp" // format::
#include "impl/init.hpp"   // init::
#include "impl/rng.hpp"    // PRNG
#include "impl/types.hpp"  // Idx, Extent, Uint, dynamic_size, Scalar, Vector<>, Matrix<>

// ____________________ DEVELOPER DOCS ____________________

// Public header with only the core functionality (matrix & matrix operations).

// ____________________ IMPLEMENTATION ____________________

namespace gse {

// ===================
// --- Basic Types ---
// ===================

using impl::Idx;
using impl::Uint;
using impl::Extent;
using impl::dynamic_size;
using impl::Scalar;
using impl::Vector;
using impl::Matrix;

// ==============================
// --- Initializing functions ---
// ==============================

using impl::PRNG;

namespace init = impl::init;

// ==================
// --- Formatting ---
// ==================

using impl::to_std;

namespace format = impl::format;

// ===============
// --- Helpers ---
// ===============

// template <class Mat>
// constexpr Extent extent_rows = static_cast<Extent>(Mat::CompileTimeTraits::RowsAtCompileTime);

// template <class Mat>
// constexpr Extent extent_cols = static_cast<Extent>(Mat::CompileTimeTraits::ColsAtCompileTime);

} // namespace gse
