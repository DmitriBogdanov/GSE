// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "./impl/jacobian/solver.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Header exposing the public API.

// ____________________ IMPLEMENTATION ____________________

namespace gse::jacobian {

using impl::jacobian::solve;

namespace defaults = impl::jacobian::defaults;
namespace method   = impl::jacobian::method;

} // namespace gse::jacobian
