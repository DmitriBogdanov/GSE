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

// ____________________ DEVELOPER DOCS ____________________

// Initializing functions for vectors & matrices.
//
// Eigen has similar methods in its API ('::Zero()', '::Constant()', '::LinSpaced()', '::Random()'),
// but here we expand the list and provide some more customization points under a GSE-style API so
// users don't have to mix coding conventions.

// ____________________ IMPLEMENTATION ____________________

namespace gse::init {

// [ LSP documentation ]
//
// Creates a zero-filled `Vector<T, N>` with `rows` elements.
//
// If `N` is known at compile-time, `rows` parameter is ignored.
//
template <scalar T = double, Extent N = dynamic>
[[nodiscard]] Vector<T, N> zero(Idx rows) {
    assert(rows >= 0);
    return Vector<T, N>::Zero(rows);
}

// [ LSP documentation ]
//
// Creates a zero-filled `Matrix<T, N, M>` with `rows` x `cols` elements.
//
// If `M` / `N` is known at compile-time, `rows` / `cols` parameter is ignored.
//
template <scalar T = double, Extent N = dynamic, Extent M = dynamic>
[[nodiscard]] Matrix<T, N, M> zero(Idx rows, Idx cols) {
    assert(rows >= 0 && cols >= 0);
    return Matrix<T, N, M>::Zero(rows, cols);
}

// [ LSP documentation ]
//
// Creates a `Vector<T, N>` with `rows` elements filled with `value`.
//
// If `N` is known at compile-time, `rows` parameter is ignored.
//
template <scalar T = double, Extent N = dynamic>
[[nodiscard]] Vector<T, N> fill(Idx rows, T value) {
    assert(rows >= 0);
    return Vector<T, N>::Constant(rows, value);
}

// [ LSP documentation ]
//
// Creates a `Matrix<T, N, M>` with `rows` x `cols` elements filled with `value`.
//
// If `M` / `N` is known at compile-time, `rows` / `cols` parameter is ignored.
//
template <scalar T = double, Extent N = dynamic, Extent M = dynamic>
[[nodiscard]] Matrix<T, N, M> fill(Idx rows, Idx cols, T value) {
    assert(rows >= 0 && cols >= 0);
    return Matrix<T, N, M>::Zero(rows, cols, value);
}

// [ LSP documentation ]
//
// Creates a `Vector<T, N>` with `rows` elements filled with random values from `dist` generated with `gen`.
//
// If `N` is known at compile-time, `rows` parameter is ignored.
//
template <scalar T = double, Extent N = dynamic, random_distribution Dist = UniformDistribution<T>,
          random_number_generator Gen = PRNG>
[[nodiscard]] Vector<T, N> random(Idx rows, Dist&& dist = Dist{}, Gen&& gen = Gen{}) {
    // By passing distribution & PRNG this way we make function more semantically explicit than Eigen's default API:
    //    > random(17, 0, 1);                                 // what kind of distribution do we have?
    //    > random(17, std::uniform_real_distribution{0, 1}); // this is clear
    // It is also suitable for all kinds of distributions & engines. Engine argument can be used for seeding:
    //    > random(17, std::normal_distribution{}, gse::PRNG{7}); // seed random with '17'
    // or to pass an existing PRNG, this way we have full control and no implicit global state.

    assert(rows >= 0);

    Vector<T, N> res = zero<T, N>(rows);
    for (auto& e : res) e = dist(gen);
    return res;
}

// [ LSP documentation ]
//
// Creates a `Matrix<T, N, M>` with `rows` x `cols` elements filled with random values from `dist` generated with `gen`.
//
// If `M` / `N` is known at compile-time, `rows` / `cols` parameter is ignored.
//
template <scalar T = double, Extent N = dynamic, Extent M = dynamic, random_distribution Dist = UniformDistribution<T>,
          random_number_generator Gen = PRNG>
[[nodiscard]] Matrix<T, N, M> random(Idx rows, Idx cols, Dist&& dist = Dist{}, Gen&& gen = Gen{}) {
    assert(rows >= 0 && cols >= 0);

    Matrix<T, N, M> res = zero<T, N, M>(rows, cols);

    for (auto& e : res.reshaped()) e = dist(gen);
    // Eigen matrices do not provide iterators, but Eigen vectors do, to do a range-based
    // loop over a matrix we can use reshaped view and treat it like a vector

    return res;
}

// [ LSP documentation ]
//
// Creates a `Vector<T, N>` with `rows` elements filled with linearly spaced values from `min` to `max`.
//
// If `N` is known at compile-time, `rows` parameter is ignored.
//
template <scalar T = double, Extent N = dynamic>
[[nodiscard]] Vector<T, N> linspace(Idx rows, T min, T max) {
    assert(rows >= 0);
    return Vector<T, N>::LinSpaced(rows, min, max);
}

// [ LSP documentation ]
//
// Creates an identity `Matrix<T, N, M>` with `rows` x `cols` elements.
//
// If `M` / `N` is known at compile-time, `rows` / `cols` parameter is ignored.
//
template <scalar T = double, Extent N = dynamic>
[[nodiscard]] Matrix<T, N, N> identity(Idx rows, Idx cols) {
    assert(rows >= 0 && cols >= 0);
    return Matrix<T, N, N>::Identity(rows, cols);
}

} // namespace gse::init