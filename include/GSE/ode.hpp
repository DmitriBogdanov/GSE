// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "./impl/ode/solver.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Header exposing the public API.

// ____________________ IMPLEMENTATION ____________________

namespace gse::ode {

using impl::ode::solve;

namespace defaults = impl::ode::defaults;
namespace base     = impl::ode::base;
namespace method   = impl::ode::method;

} // namespace gse::ode
