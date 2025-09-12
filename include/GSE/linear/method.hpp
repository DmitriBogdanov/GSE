// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "GSE/core/concepts.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Common constrains & functionality of linear methods.

// ____________________ IMPLEMENTATION ____________________

namespace gse::linear {

// [ LSP documentation ]
//
// Concept that constrains `Method` to a signature of a linear method:
//    `Vector<T, N> ( Matrix<T, N, N>, Vector<T, N> )`
//
template <class Method, class T, Extent N>
concept method_function = invocable_r<Method, Vector<T, N>, Matrix<T, N, N>, Vector<T, N>>;

} // namespace gse::linear
