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

#include "./rng.hpp"
#include "./types.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Initializing functions for vectors & matrices.
//
// Eigen has similar methods in its API ('::Zero()', '::Constant()', '::LinSpaced()', '::Random()'),
// but their documentation is somewhat obscure and easy to forget, here we have all of them in a dedicated
// namespace with some additional improvements (faster RNG, more control points, more explicit API, etc.).

// ____________________ IMPLEMENTATION ____________________

// ======================
// --- Initialization ---
// ======================

namespace gse::impl::init {

template <class T, Extent N = dynamic>
[[nodiscard]] Vector<T, N> zero(Idx rows) {
    assert(rows >= 0);
    return Vector<T, N>::Zero(rows);
}

template <class T, Extent N = dynamic, Extent M = dynamic>
[[nodiscard]] Matrix<T, N, M> zero(Idx rows, Idx cols) {
    assert(rows >= 0 && cols >= 0);
    return Matrix<T, N, M>::Zero(rows, cols);
}

template <class T, Extent N = dynamic>
[[nodiscard]] Vector<T, N> fill(Idx rows, T value) {
    assert(rows >= 0);
    return Vector<T, N>::Constant(rows, value);
}

template <class T, Extent N = dynamic, Extent M = dynamic>
[[nodiscard]] Matrix<T, N, M> fill(Idx rows, Idx cols, T value) {
    assert(rows >= 0 && cols >= 0);
    return Matrix<T, N, M>::Zero(rows, cols, value);
}

template <class T, Extent N = dynamic, class Dist = UniformDistribution<T>, class Gen = PRNG>
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

template <class T, Extent N = dynamic, Extent M = dynamic, class Dist = UniformDistribution<T>, class Gen = PRNG>
[[nodiscard]] Matrix<T, N, M> random(Idx rows, Idx cols, Dist&& dist = Dist{}, Gen&& gen = Gen{}) {
    assert(rows >= 0 && cols >= 0);

    Matrix<T, N, M> res = zero<T, N, M>(rows, cols);

    for (auto& e : res.reshaped()) e = dist(gen);
    // Eigen matrices do not provide iterators, but Eigen vectors do, to do a range-based 
    // loop over a matrix we can use reshaped view and treat it like a vector
    
    return res;
}

template <class T, Extent N = dynamic>
[[nodiscard]] Vector<T, N> linspace(Idx rows, T min, T max) {
    assert(rows >= 0);
    return Vector<T, N>::LinSpaced(rows, min, max);
}

template <class T, Extent N = dynamic>
[[nodiscard]] Matrix<T, N, N> identity(Idx rows, Idx cols) {
    assert(rows >= 0 && cols >= 0);
    return Matrix<T, N, N>::Identity(rows, cols);
}

template <class T, Extent N = dynamic>
[[nodiscard]] Vector<T, N> from_list(std::initializer_list<T> list) {
    // 'from_list()' also saves us from disasters like Eigen's fixed-size vs dynamic-size construction:
    //    'Vector< >(2, 1)' - vector with size '2' filled with '1'
    //    'Vector<2>(2, 1)' - vector {2, 1}
    // in GSE style we would have:
    //    'init::from_list({2, 1})' & 'init::fill(2, 1)' - now the purpose is explicit

    if constexpr (N != dynamic) assert(N == list.size());

    Vector<T, N> res = zero<T, N>(list.size());
    for (Idx i = 0; i < res.size(); ++i) res[i] = list.begin()[i];
    return res;
}

// template <class T, Extent N = dynamic, Extent M = dynamic>
// [[nodiscard]] Matrix<T, N, M> from_list(std::initializer_list<std::initializer_list<T>> row_list) {
//     // 'from_list()' also saves us from disasters like Eigen's fixed-size vs dynamic-size construction:
//     //    'Vector< >(2, 1)' - vector with size '2' filled with '1'
//     //    'Vector<2>(2, 1)' - vector {2, 1}
//     // in GSE style we would have:
//     //    'init::from_list({2, 1})' & 'init::fill(2, 1)' - now the purpose is explicit

//     if constexpr (N != dynamic) assert(N == row_list.size());
//     if constexpr (M != dynamic)
//         for (const auto& col_list : row_list) assert(M == col_list.size());

//     Matrix<T, N, M> res = zero<T, N, M>(row_list.size(), ro);
//     for (Idx i = 0; i < res.size(); ++i) res[i] = row_list.begin()[i];
//     return res;
// }

} // namespace gse::impl::init
