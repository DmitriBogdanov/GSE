// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "./types.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Type conversions and IO formats useful for matrix serializing.

// ____________________ IMPLEMENTATION ____________________

// ==================
// --- Conversion ---
// ==================

namespace gse::impl {

// Convert 'Vector<N>' to std-containers
template <class T, Extent N>
auto to_std(const Vector<T, N>& vec) {
    // Size is dynamic => convert to 'std::vector<>'
    if constexpr (N == dynamic) {
        return std::vector<T>(vec.data(), vec.data() + vec.size());
    }
    // Size is static  => convert to 'std::array<>'
    else {
        std::array<T, N> res;
        for (std::size_t i = 0; i < N; ++i) res[i] = vec[i];
        return res;
    }
}

// Convert 'Matrix<N, M>' to std-containers
template <class T, Extent N, Extent M>
auto to_std(const Matrix<T, N, M>& mat) {
    // Lambda to summarize fill loop that would be 4x duplicated otherwise
    const auto fill = [&](auto& std_mat) {
        for (Idx i = 0; i < mat.rows(); ++i)
            for (Idx j = 0; j < mat.cols(); ++j) std_mat[i][j] = mat(i, j);

        return std_mat;
    };

    // Size is { dynamic, dynamic } => convert to 'std::vector<std::vector<>>'
    if constexpr (N == dynamic && M == dynamic) {
        std::vector<std::vector<T>> res(mat.rows());
        for (Idx i = 0; i < mat.rows(); ++i) res[i].resize(mat.cols());
        return fill(mat);
    }
    // Size is { dynamic, static } => convert to 'std::vector<std::array<>>'
    else if constexpr (N == dynamic) {
        std::vector<std::array<T, M>> res(mat.rows());
        return fill(mat);
    }
    // Size is { static, dynamic } => convert to 'std::array<std::vector<>>'
    else if constexpr (M == dynamic) {
        std::array<std::vector<T>, N> res;
        for (Idx i = 0; i < mat.rows(); ++i) res[i].resize(mat.cols());
        return fill(mat);
    }
    // Size is { static, static } => convert to 'std::array<std::array>'
    else {
        std::array<std::array<T, M>, N> res;
        return fill(mat);
    }
}

} // namespace gse::impl

// =================
// --- IO Format ---
// =================

namespace gse::impl::format {

inline const Eigen::IOFormat matrix(6, 0, ", ", "\n", "[", "]");
inline const Eigen::IOFormat vector(6, 0, ", ", ", ", "", "", "{", "}");
inline const Eigen::IOFormat none(6, 0, " ", " ", "", "", "", "");

} // namespace format
