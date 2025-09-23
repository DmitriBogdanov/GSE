// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "GSE/core/types.hpp"

#include <type_traits> // enable_if_t<>

// ____________________ DEVELOPER DOCS ____________________

// SFINAE helpers for use cases that can't be managed with concepts.

// ____________________ IMPLEMENTATION ____________________

namespace gse::impl {

template <bool Cond>
using require = std::enable_if_t<Cond, bool>;

template <Extent N>
using require_dynamic = require<N == dynamic>;

template <Extent N>
using require_static = require<N != dynamic>;

template <Extent N, Extent M>
using require_dynamic_matrix = require<N == dynamic || M == dynamic>;

template <Extent N, Extent M>
using require_static_matrix = require<N != dynamic && M != dynamic>;

} // namespace gse::impl
