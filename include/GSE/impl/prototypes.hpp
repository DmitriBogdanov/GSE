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

// Template "dummy functions" used by the method concepts.

// ____________________ IMPLEMENTATION ____________________

namespace gse::impl::prototypes {

template <scalar T>
struct ScalarFunction {
    T operator()(T) { return T{}; }
};

template <scalar T, Extent N>
struct MultivariateFunction {
    T operator()(Vector<T, N>) { return T{}; }
};

template <scalar T, Extent N>
struct VectorFunction {
    Vector<T, N> operator()(Vector<T, N>) { return Vector<T, N>{}; }
};

} // namespace gse::impl::prototypes