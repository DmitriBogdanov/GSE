// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include <random> // uniform_real_distribution<>

#include "core.hpp"
#include "rng.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Different initializing methods for vectors & matrices
//
// Eigen has similar methods in its API ('::Zero()', '::Constant()', '::LinSpaced()', '::Random()'),
// but their documentation is somewhat obscure and easy to forget, here we have all of them in a
// dedicated namespace with some additional improvements (like faster PRNG)
//
// 'from_list()' also saves us from disasters like Eigen's fixed-size vs dynamic-size construction:
//    'Vector< >(2, 1)' - vector with size '2' filled with '1'
//    'Vector<2>(2, 1)' - vector {2, 1}
// in GSE style we would have:
//    'init::from_list({2, 1})' & 'init::fill(2, 1)' - now the purpose is explicit


// ____________________ IMPLEMENTATION ____________________

// ======================
// --- Initialization ---
// ======================

namespace gse::init {

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

template <Extent N = dynamic_size>
[[nodiscard]] Vector<N> random(Idx rows, Scalar min = 0, Scalar max = 1) {
    assert(rows >= 0);

    Vector<N> res = zero<N>(rows);

    std::uniform_real_distribution<Scalar> dist{min, max};
    for (const auto& e : res) e = dist(rng::default_generator);
    return res;
}

template <Extent N = dynamic_size, Extent M = dynamic_size>
[[nodiscard]] Matrix<N, M> random(Idx rows, Idx cols, Scalar min = 0, Scalar max = 1) {
    assert(rows >= 0 && cols >= 0);

    Matrix<N, M> res = zero<N, M>(rows, cols);

    std::uniform_real_distribution<Scalar> dist{min, max};
    for (const auto& e : res) e = dist(rng::default_generator);
    return res;
}

template <Extent N = dynamic_size>
[[nodiscard]] Vector<N> linspace(Idx rows, Scalar min, Scalar max) {
    assert(rows >= 0);
    return Vector<N>::LinSpaced(rows, min, max);
}

[[nodiscard]] inline Vector<> from_list(std::initializer_list<Scalar> init_list) {
    Vector<> res = zero(init_list.size());
    for (Idx i = 0; i < res.size(); ++i) res[i] = init_list.begin()[i];
    return res;
}

} // namespace gse::init
