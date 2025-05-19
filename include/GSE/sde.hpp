// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "./impl/sde/solver.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Header exposing the public API.

// ____________________ IMPLEMENTATION ____________________

namespace gse::sde {

using impl::sde::solve;

namespace defaults = impl::sde::defaults;
namespace base     = impl::sde::base;
namespace method   = impl::sde::method;

} // namespace gse::sde
