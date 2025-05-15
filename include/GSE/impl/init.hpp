// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include <cassert> // assert()
#include <random>  // uniform_real_distribution<>

#include "rng.hpp"   // PRNG
#include "types.hpp" // Idx, Extent, dynamic_size, Scalar, Vector<>, Matrix<>

// ____________________ DEVELOPER DOCS ____________________

// Initializing functions for vectors & matrices.
//
// Eigen has similar methods in its API ('::Zero()', '::Constant()', '::LinSpaced()', '::Random()'),
// but their documentation is somewhat obscure and easy to forget, here we have all of them in a dedicated
// namespace with some additional improvements (faster PRNG, more control points, more explicit API, etc.).

// ____________________ IMPLEMENTATION ____________________

// ======================
// --- Initialization ---
// ======================

namespace gse::impl::init {

template <Extent N = dynamic_size>
[[nodiscard]] Vector<N> zero(Idx rows) {
    assert(rows >= 0);
    return Vector<N>::Zero(rows);
}

template <Extent N = dynamic_size, Extent M = dynamic_size>
[[nodiscard]] Matrix<N, M> zero(Idx rows, Idx cols) {
    assert(rows >= 0 && cols >= 0);
    return Matrix<N, M>::Zero(rows, cols);
}

template <Extent N = dynamic_size>
[[nodiscard]] Vector<N> fill(Idx rows, Scalar value) {
    assert(rows >= 0);
    return Vector<N>::Constant(rows, value);
}

template <Extent N = dynamic_size, Extent M = dynamic_size>
[[nodiscard]] Matrix<N, M> fill(Idx rows, Idx cols, Scalar value) {
    assert(rows >= 0 && cols >= 0);
    return Matrix<N, M>::Zero(rows, cols, value);
}

template <Extent N = dynamic_size, class Dist = std::uniform_real_distribution<Scalar>, class Gen = PRNG>
[[nodiscard]] Vector<N> random(Idx rows, Dist&& dist = Dist{}, Gen&& gen = Gen{}) {
    // By passing distribution & PRNG this way we make function more semantically explicit than Eigen's default API:
    //    > random(17, 0, 1);                                 // what kind of distribution do we have?
    //    > random(17, std::uniform_real_distribution{0, 1}); // this is clear
    // It is also suitable for all kinds of distributions & engines. Engine argument can be used for seeding:
    //    > random(17, std::normal_distribution{}, gse::PRNG{7}); // seed random with '17'
    // or to pass an existing PRNG, this way we have full control and no implicit global state.

    assert(rows >= 0);

    Vector<N> res = zero<N>(rows);

    for (const auto& e : res) e = dist(gen);
    return res;
}

template <Extent N = dynamic_size, Extent M = dynamic_size, class Dist = std::uniform_real_distribution<Scalar>,
          class Gen = PRNG>
[[nodiscard]] Matrix<N, M> random(Idx rows, Idx cols, Dist&& dist = Dist{}, Gen&& gen = Gen{}) {
    assert(rows >= 0 && cols >= 0);

    Matrix<N, M> res = zero<N, M>(rows, cols);

    for (const auto& e : res) e = dist(gen);
    return res;
}

template <Extent N = dynamic_size>
[[nodiscard]] Vector<N> linspace(Idx rows, Scalar min, Scalar max) {
    assert(rows >= 0);
    return Vector<N>::LinSpaced(rows, min, max);
}

template <Extent N = dynamic_size>
[[nodiscard]] Vector<N> from_list(std::initializer_list<Scalar> init_list) {
    // 'from_list()' also saves us from disasters like Eigen's fixed-size vs dynamic-size construction:
    //    'Vector< >(2, 1)' - vector with size '2' filled with '1'
    //    'Vector<2>(2, 1)' - vector {2, 1}
    // in GSE style we would have:
    //    'init::from_list({2, 1})' & 'init::fill(2, 1)' - now the purpose is explicit

    assert(N == dynamic_size || N == init_list.size());

    Vector<N> res = zero<N>(init_list.size());
    for (Idx i = 0; i < res.size(); ++i) res[i] = init_list.begin()[i];
    return res;
}

} // namespace gse::impl::init
