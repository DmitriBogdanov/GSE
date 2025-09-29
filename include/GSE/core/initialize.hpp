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
#include "GSE/core/random.hpp"
#include "GSE/impl/sfinae.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Initializing functions for vectors & matrices.
//
// Eigen has similar methods in its API ('::Zero()', '::Constant()', '::LinSpaced()', '::Random()'),
// but here we expand the list and provide some more customization points under a GSE-style API so
// users don't have to mix coding conventions.
//
// This also allows for more compile-time extent checking and some general API improvements,
// mainly in the 'init::random()'.

// ____________________ IMPLEMENTATION ____________________

namespace gse::init {

// ===================
// --- Zero vector ---
// ===================

// [ LSP documentation ]
//
// Creates a zero-filled `Vector<T, N>` with `rows` elements.
//
// For static `N`, parameter `rows` is optional.
//
template <scalar T = double, Extent N = dynamic, impl::require_dynamic<N> = true>
[[nodiscard]] Vector<T, N> zero(Idx rows) {
    assert(rows >= 0);
    return Vector<T, N>::Zero(rows);
}

// [ LSP documentation ]
//
// Creates a zero-filled `Vector<T, N>` with `rows` elements.
//
// For static `N`, parameter `rows` is optional.
//
template <scalar T = double, Extent N = dynamic, impl::require_static<N> = true>
[[nodiscard]] Vector<T, N> zero(Idx rows = N) {
    assert(N == rows);
    assert(rows >= 0);
    return Vector<T, N>::Zero(rows);
}

// Note: This duplication is necessary to provide a nice API with default parameters for statically sized vectors.

// ===================
// --- Zero matrix ---
// ===================

// [ LSP documentation ]
//
// Creates a zero-filled `Matrix<T, N, M>` with `rows` x `cols` elements.
//
// For static `N` / `M`, parameters `rows` / `cols` are optional.
//
template <scalar T = double, Extent N = dynamic, Extent M = dynamic, impl::require_dynamic_matrix<N, M> = true>
[[nodiscard]] Matrix<T, N, M> zero(Idx rows, Idx cols) {
    assert(rows >= 0);
    assert(cols >= 0);
    return Matrix<T, N, M>::Zero(rows, cols);
}

// [ LSP documentation ]
//
// Creates a zero-filled `Matrix<T, N, M>` with `rows` x `cols` elements.
//
// For static `N` / `M`, parameters `rows` / `cols` are optional.
//
template <scalar T = double, Extent N = dynamic, Extent M = dynamic, impl::require_static_matrix<N, M> = true>
[[nodiscard]] Matrix<T, N, M> zero(Idx rows = N, Idx cols = M) {
    assert(N == rows);
    assert(M == cols);
    assert(rows >= 0);
    assert(cols >= 0);
    return Matrix<T, N, M>::Zero(rows, cols);
}

// Note: We could support mixed case (dynamic rows & static cols) with cols defaulted,
//       but this would lead to a somewhat error-prone API so we restrict it.

// =======================
// --- Constant vector ---
// =======================

// [ LSP documentation ]
//
// Creates a `Vector<T, N>` with `rows` elements filled with `value`.
//
// For static `N`, parameter `rows` should correspond to the static value.
//
template <scalar T = double, Extent N = dynamic, impl::require_dynamic<N> = true>
[[nodiscard]] Vector<T, N> constant(Idx rows, T value) {
    assert(N == dynamic || N == rows);
    assert(rows >= 0);
    return Vector<T, N>::Constant(rows, value);
}

// =======================
// --- Constant matrix ---
// =======================

// [ LSP documentation ]
//
// Creates a `Matrix<T, N, M>` with `rows` x `cols` elements filled with `value`.
//
// For static `N` / `M`, parameters `rows` / `cols` should correspond to the static value.
//
template <scalar T = double, Extent N = dynamic, Extent M = dynamic>
[[nodiscard]] Matrix<T, N, M> constant(Idx rows, Idx cols, T value) {
    assert(N == dynamic || N == rows);
    assert(M == dynamic || M == cols);
    assert(rows >= 0);
    assert(cols >= 0);
    return Matrix<T, N, M>::Zero(rows, cols, value);
}

// =====================
// --- Random vector ---
// =====================

// [ LSP documentation ]
//
// Creates a `Vector<T, N>` with `rows` elements filled with random values from `dist` generated with `gen`.
//
// For static `N`, parameter `rows` is optional.
//
template <scalar T = double, Extent N = dynamic, random_distribution Dist = UniformDistribution<T>,
          random_number_generator Gen = PRNG, impl::require_dynamic<N> = true>
[[nodiscard]] Vector<T, N> random(Idx rows, Dist&& dist = Dist{}, Gen&& gen = Gen{}) {
    // By passing distribution & PRNG this way we make function more semantically explicit than Eigen's default API:
    //    > random(17, 0, 1);                                 // what kind of distribution do we have?
    //    > random(17, std::uniform_real_distribution{0, 1}); // this is clear
    // It is also suitable for all kinds of distributions & engines. Engine argument can be used for seeding:
    //    > random(17, std::normal_distribution{}, gse::PRNG{7}); // seed random with '17'
    // or to pass an existing PRNG, this way we have full control and no implicit global state.

    assert(N == dynamic || N == rows);
    assert(rows >= 0);

    Vector<T, N> res = zero<T, N>(rows);
    for (auto& e : res) e = dist(gen);
    return res;
}

// [ LSP documentation ]
//
// Creates a `Vector<T, N>` with `rows` elements filled with random values from `dist` generated with `gen`.
//
// For static `N`, parameter `rows` is optional.
//
template <scalar T = double, Extent N = dynamic, random_distribution Dist = UniformDistribution<T>,
          random_number_generator Gen = PRNG, impl::require_static<N> = true>
[[nodiscard]] Vector<T, N> random(Idx rows = N, Dist&& dist = Dist{}, Gen&& gen = Gen{}) {
    assert(N == rows);
    assert(rows >= 0);

    Vector<T, N> res = zero<T, N>(rows);
    for (auto& e : res) e = dist(gen);
    return res;
}

// =====================
// --- Random matrix ---
// =====================

// [ LSP documentation ]
//
// Creates a `Matrix<T, N, M>` with `rows` x `cols` elements filled with random values from `dist` generated with `gen`.
//
// For static `N` / `M`, parameters `rows` / `cols` are optional.
//
template <scalar T = double, Extent N = dynamic, Extent M = dynamic, random_distribution Dist = UniformDistribution<T>,
          random_number_generator Gen = PRNG, impl::require_dynamic_matrix<N, M> = true>
[[nodiscard]] Matrix<T, N, M> random(Idx rows, Idx cols, Dist&& dist = Dist{}, Gen&& gen = Gen{}) {
    assert(N == dynamic || N == rows);
    assert(M == dynamic || M == cols);
    assert(rows >= 0);
    assert(cols >= 0);

    Matrix<T, N, M> res = zero<T, N, M>(rows, cols);

    for (auto& e : res.reshaped()) e = dist(gen);
    // Eigen matrices do not provide iterators, but Eigen vectors do, to do a range-based
    // loop over a matrix we can use reshaped view and treat it like a vector

    return res;
}

// [ LSP documentation ]
//
// Creates a `Matrix<T, N, M>` with `rows` x `cols` elements filled with random values from `dist` generated with `gen`.
//
// For static `N` / `M`, parameters `rows` / `cols` are optional.
//
template <scalar T = double, Extent N = dynamic, Extent M = dynamic, random_distribution Dist = UniformDistribution<T>,
          random_number_generator Gen = PRNG, impl::require_static_matrix<N, M> = true>
[[nodiscard]] Matrix<T, N, M> random(Idx rows = N, Idx cols = M, Dist&& dist = Dist{}, Gen&& gen = Gen{}) {
    assert(N == rows);
    assert(M == cols);
    assert(rows >= 0);
    assert(cols >= 0);

    Matrix<T, N, M> res = zero<T, N, M>(rows, cols);

    for (auto& e : res.reshaped()) e = dist(gen);
    // Eigen matrices do not provide iterators, but Eigen vectors do, to do a range-based
    // loop over a matrix we can use reshaped view and treat it like a vector

    return res;
}

// =======================
// --- Linspace vector ---
// =======================

// [ LSP documentation ]
//
// Creates a `Vector<T, N>` with `rows` elements filled with linearly spaced values from `min` to `max`.
//
// For static `N`, parameter `rows` should correspond to the static value.
//
template <scalar T = double, Extent N = dynamic>
[[nodiscard]] Vector<T, N> linspace(Idx rows, T min, T max) {
    assert(rows >= 0);
    assert(N == dynamic || N == rows);
    return Vector<T, N>::LinSpaced(rows, min, max);
}

// =======================
// --- Identity matrix ---
// =======================

// [ LSP documentation ]
//
// Creates an identity `Matrix<T, N, M>` with `rows` x `cols` elements.
//
// For static `N` / `M`, parameters `rows` / `cols` are optional.
//
template <scalar T = double, Extent N = dynamic, Extent M = dynamic, impl::require_dynamic_matrix<N, M> = true>
[[nodiscard]] Matrix<T, N, M> identity(Idx rows, Idx cols) {
    assert(rows >= 0);
    assert(cols >= 0);
    return Matrix<T, N, M>::Identity(rows, cols);
}

// [ LSP documentation ]
//
// Creates an identity `Matrix<T, N, M>` with `rows` x `cols` elements.
//
// For static `N` / `M`, parameters `rows` / `cols` are optional.
//
template <scalar T = double, Extent N = dynamic, Extent M = dynamic, impl::require_static_matrix<N, M> = true>
[[nodiscard]] Matrix<T, N, M> identity(Idx rows = N, Idx cols = M) {
    assert(N == rows);
    assert(M == cols);
    assert(rows >= 0);
    assert(cols >= 0);
    return Matrix<T, N, M>::Identity(rows, cols);
}

} // namespace gse::init