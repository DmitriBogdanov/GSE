// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "GSE/core/types.hpp"

#include <concepts>    // invocable<>, convertible_to<>
#include <functional>  // invoke()
#include <type_traits> // is_convertible_v<>
#include <utility>     // forward()

// ____________________ DEVELOPER DOCS ____________________

// All the core concepts for constraining matrices, functions and their dimensions.

// ____________________ IMPLEMENTATION ____________________

namespace gse {

// clang-format off
// Note: 'clangd' doesn't handle concept indentation that well

// ===========================
// --- Generic constraints ---
// ===========================

// Refinement of the 'std::convertible_to<>' specialized to prohibit conversions between statically
// sized matrices that would be dimensionally incorrect. For other types works like a regular concept.

namespace impl {

// Non-matrix case    
template <class From, class To>
constexpr bool is_convertible_v = std::is_convertible_v<From, To>;

// Dimensions don't match
template <class T1, class T2, Extent M1, Extent M2, Extent N1, Extent N2>
constexpr bool is_convertible_v<Matrix<T1, M1, N1>, Matrix<T2, M2, N2>> =
    std::is_convertible_v<T1, T2>
    &&
    !(
        (M1 != dynamic && M2 != dynamic && M1 != M2) // rows are static & don't match
        ||
        (N1 != dynamic && N2 != dynamic && N1 != N2) // cols are static & don't match
    )
;

}

template <class From, class To>
concept convertible_to = impl::is_convertible_v<From, To>;

template <class Func, class R, class... Args>
concept invocable_r = std::invocable<Func, Args...> && requires(Func&& f, Args&&... args) {
    { std::invoke(f, std::forward<Args>(args)...) } -> convertible_to<R>;
    // will catch dimension error due to the specialized concept
};

// ========================
// --- Type constraints ---
// ========================

template <class T>
concept scalar = std::floating_point<T> || std::integral<T>; // temporary solution

template <class Mat>
concept vector = scalar<typename Mat::value_type> && (Mat::ColsAtCompileTime == 1) && requires() {
    { Mat::RowsAtCompileTime } -> std::convertible_to<Extent>;
    { Mat::ColsAtCompileTime } -> std::convertible_to<Extent>;
    // 'RowsAtCompileTime' / 'ColsAtCompileTime' have an unscoped enum type 'Mat::CompileTimeTraits',
    // constraining to 'same_as<Extent>' would be too strict
};

template <class Mat>
concept matrix = scalar<typename Mat::value_type> && requires() {
    { Mat::RowsAtCompileTime } -> std::convertible_to<Extent>;
    { Mat::ColsAtCompileTime } -> std::convertible_to<Extent>;
};

// ========================
// --- Size constraints ---
// ========================

template <class Mat, Extent M>
concept rows_equal_to = (Mat::RowsAtCompileTime == M);

template <class Mat, Extent N>
concept cols_equal_to = (Mat::ColsAtCompileTime == N);

// ============================
// --- Function constraints ---
// ============================

template <class Func, class T>
concept scalar_function = scalar<T> && invocable_r<Func, T, T>;

template <class Func, class T, Extent N>
concept vector_function = scalar<T> && invocable_r<Func, Vector<T, N>, Vector<T, N>>;

template <class Func, class T, Extent N>
concept multivariate_function = scalar<T> && invocable_r<Func, T, Vector<T, N>>;

// clang-format on

} // namespace gse
