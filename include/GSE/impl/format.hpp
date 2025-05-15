// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "types.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Type conversion and IO formats useful for matrix serializing.

// ____________________ IMPLEMENTATION ____________________

// ==================
// --- Conversion ---
// ==================

namespace gse::impl {

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

} // namespace gse::impl

// =================
// --- IO Format ---
// =================

namespace gse::impl::format {

inline const Eigen::IOFormat matrix(6, 0, ", ", "\n", "[", "]");
inline const Eigen::IOFormat vector(6, 0, ", ", ", ", "", "", "{", "}");
inline const Eigen::IOFormat none(6, 0, " ", " ", "", "", "", "");

} // namespace format
