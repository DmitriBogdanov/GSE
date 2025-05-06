// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include <array>            // array<>
#include <cstddef>          // size_t
#include <functional>       // function
#include <initializer_list> // initializer_list<>
#include <type_traits>      // is_invocable<>, is_invocable_r<>, is_convertible<>
#include <vector>           // vector<>

#include "thirdparty/Eigen/Dense" // Matrix, Vector, Dynamic

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

// ======================
// --- Initialization --- // TODO: Move to a different header, add random initialization with a better PRNG
// ======================

namespace init {

template <Extent N = dynamic_size>
gse::Vector<N> zero(gse::Idx rows) {
    assert(rows >= 0);
    return gse::Vector<N>::Zero(rows);
}

template <Extent N = dynamic_size, Extent M = dynamic_size>
gse::Matrix<N, M> zero(gse::Idx rows, gse::Idx cols) {
    assert(rows >= 0 && cols >= 0);
    return gse::Matrix<N, M>::Zero(rows, cols);
}

} // namespace init

// ===============
// --- Helpers ---
// ===============

template <class Mat>
constexpr Extent extent_rows = static_cast<Extent>(Mat::CompileTimeTraits::RowsAtCompileTime);

template <class Mat>
constexpr Extent extent_cols = static_cast<Extent>(Mat::CompileTimeTraits::ColsAtCompileTime);

Vector<> make_vector(std::initializer_list<Scalar> init_list) {
    Vector<> res = Vector<>::Zero(init_list.size());
    for (Idx i = 0; i < res.size(); ++i) res[i] = init_list.begin()[i];
    return res;
}

// ==================
// --- Formatting ---
// ==================

// Convert 'Vector<N>' to std-containers
template <Extent N>
auto to_std(const Vector<N>& vec) {
    // Size is dynamic => convert to 'std::vector<>'
    if constexpr (N == dynamic_size) {
        return std::vector<Scalar>(vec.data(), vec.data() + vec.size());
    }
    // Size is static  => convert to 'std::array<>'
    else {
        std::array<Scalar, N> res;
        for (std::size_t i = 0; i < N; ++i) res[i] = vec[i];
        return res;
    }
} 

// Convert 'Matrix<N, M>' to std-containers
template <Extent N, Extent M>
auto to_std(const Matrix<N, M>& mat) {
    // Lambda to summarize fill loop that would be 4x duplicated otherwise
    const auto fill = [&](auto& std_mat) {
        for (Idx i = 0; i < mat.rows(); ++i)
            for (Idx j = 0; j < mat.cols(); ++j) std_mat[i][j] = mat(i, j);

        return std_mat;
    };

    // Size is { dynamic, dynamic } => convert to 'std::vector<std::vector<>>'
    if constexpr (N == dynamic_size && M == dynamic_size) {
        std::vector<std::vector<Scalar>> res(mat.rows());
        for (Idx i = 0; i < mat.rows(); ++i) res[i].resize(mat.cols());
        return fill(mat);
    }
    // Size is { dynamic, static } => convert to 'std::vector<std::array<>>'
    else if constexpr (N == dynamic_size) {
        std::vector<std::array<Scalar, M>> res(mat.rows());
        return fill(mat);
    }
    // Size is { static, dynamic } => convert to 'std::array<std::vector<>>'
    else if constexpr (M == dynamic_size) {
        std::array<std::vector<Scalar>, N> res;
        for (Idx i = 0; i < mat.rows(); ++i) res[i].resize(mat.cols());
        return fill(mat);
    }
    // Size is { static, static } => convert to 'std::array<std::array>'
    else {
        std::array<std::array<Scalar, M>, N> res;
        return fill(mat);
    }
}

namespace format {

inline const Eigen::IOFormat matrix(6, 0, ", ", "\n", "[", "]");
inline const Eigen::IOFormat vector(6, 0, ", ", ", ", "", "", "{", "}");
inline const Eigen::IOFormat none(6, 0, " ", " ", "", "", "", "");

} // namespace format

// ===================
// --- Type traits ---
// ===================

template <bool Cond>
using _require = std::enable_if_t<Cond, bool>;

template <class T, class Signature>
using _require_signature = _require<std::is_convertible_v<T, std::function<Signature>>>;

template <class T, class... Args>
using _require_invocable = _require<std::is_invocable_v<T, Args...>>;

template <class T, class... Args>
using _require_not_invocable = _require<!std::is_invocable_v<T, Args...>>;

template <class Ret, class T, class... Args>
using _require_invocable_r = _require<std::is_invocable_r_v<Ret, T, Args...>>;

} // namespace gse
