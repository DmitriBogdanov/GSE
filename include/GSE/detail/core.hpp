// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include <array>   // array<>
#include <cstddef> // size_t
#include <type_traits>
#include <vector>  // vector<>

#include "thirdparty/Eigen/Dense"

// ____________________ DEVELOPER DOCS ____________________

// TODO: DOCS

// ____________________ IMPLEMENTATION ____________________

namespace gse {

// ===================
// --- Basic Types ---
// ===================

using Idx    = Eigen::Index;             // ptrdiff_t
using Extent = decltype(Eigen::Dynamic); // int
using Uint   = unsigned int;

constexpr Extent dynamic_size = Eigen::Dynamic;

using Scalar = double;

template <Extent rows = dynamic_size>
using Vector = Eigen::Vector<Scalar, rows>;

template <Extent rows = dynamic_size, Extent cols = dynamic_size>
using Matrix = Eigen::Matrix<Scalar, rows, cols>;

// template <Extent rows = dynamic_size>
// using VectorView = typename Vector<rows>::MapType;

// template <Extent rows = dynamic_size, Extent cols = dynamic_size>
// using MatrixView = typename Matrix<rows, cols>::MapType;

// ===============
// --- Helpers ---
// ===============

template <class Mat>
constexpr Extent extent_rows = static_cast<Extent>(Mat::CompileTimeTraits::RowsAtCompileTime);

template <class Mat>
constexpr Extent extent_cols = static_cast<Extent>(Mat::CompileTimeTraits::ColsAtCompileTime);

// template<class T>
// const std::decay_t<T>& raw_cref(T&& value) {
//     return std::forward<T>(value);
// }

// template<class T>
// std::decay_t<T>& raw_ref(T&& value) {
//     return std::forward<T>(value);
// }

// ==================
// --- Formatting ---
// ==================

template <Extent N>
auto to_std(const Vector<N>& vec) {
    // Size is dynamic => convert to 'std::vector'
    if constexpr (N == dynamic_size) {
        return std::vector<Scalar>(vec.data(), vec.data() + vec.size());
    }
    // Size is static  => convert to 'std::array'
    else {
        std::array<Scalar, N> res;
        for (std::size_t i = 0; i < N; ++i) res[i] = vec[i];
        return res;
    }
} // converts 'Vector<N>' to std-containers

namespace format {

inline const Eigen::IOFormat matrix(6, 0, ", ", "\n", "[", "]");
inline const Eigen::IOFormat vector(6, 0, ", ", ", ", "", "", "{", "}");
inline const Eigen::IOFormat none(6, 0, " ", " ", "", "", "", "");

} // namespace format

// ===================
// --- Type traits ---
// ===================

template<bool Cond>
using _require = std::enable_if_t<Cond, bool>;

template<class T, class... Args>
using _require_invocable = _require<std::is_invocable_v<T, Args...>>;

template<class T, class... Args>
using _require_not_invocable = _require<!std::is_invocable_v<T, Args...>>;

template<class Ret, class T, class... Args>
using _require_invocable_r = _require<std::is_invocable_r_v<Ret, T, Args...>>;

} // namespace gse
