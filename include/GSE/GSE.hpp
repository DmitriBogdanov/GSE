// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "detail/alg.hpp"
#include "detail/ode.hpp"
#include "detail/sde.hpp"

// ____________________ DEVELOPER DOCS ____________________

// This header gathers includes for public API, below are some important notes on the usage

// [!] IMPORTANT:
// Includes Eigen as a dependency

// [!] IMPORTANT:
// Fixed-size Eigen vectors should NOT be passed by value, doing so is illegal,
// see https://eigen.tuxfamily.org/dox/group__TopicPassingByValue.html
