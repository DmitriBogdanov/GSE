// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "./impl/core/format.hpp"
#include "./impl/core/init.hpp"
#include "./impl/core/rng.hpp"
#include "./impl/core/types.hpp"

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
using impl::dynamic;
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

} // namespace gse
