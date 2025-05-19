// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "./impl/linear/solver.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Header exposing the public API.

// ____________________ IMPLEMENTATION ____________________

namespace gse::linear {

using impl::linear::solve;

namespace defaults = impl::linear::defaults;
namespace method   = impl::linear::method;

} // namespace gse::linear
