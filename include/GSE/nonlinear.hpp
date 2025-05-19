// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "./impl/nonlinear/solver.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Header exposing the public API.

// ____________________ IMPLEMENTATION ____________________

namespace gse::nonlinear {

using impl::nonlinear::solve;

namespace defaults = impl::nonlinear::defaults;
namespace method   = impl::nonlinear::method;

} // namespace gse::nonlinear
