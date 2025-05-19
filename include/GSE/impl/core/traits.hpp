// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include <cstdint>     // uint32_t
#include <functional>  // function<>
#include <type_traits> // enable_if_t<>, is_invocable<>, is_invocable_r<>, is_convertible<>, is_arithmetic<>, ...
#include <utility>     // declval<>()

#include "./types.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Type traits & SFINAE helpers.

// ____________________ IMPLEMENTATION ____________________

namespace gse::impl {

// ===================
// --- Type traits ---
// ===================

#define GSE_IMPL_DEFINE_TRAIT(trait_name_, ...)                                                                        \
    template <class T, class = void>                                                                                   \
    struct trait_name_ : std::false_type {};                                                                           \
                                                                                                                       \
    template <class T>                                                                                                 \
    struct trait_name_<T, std::void_t<decltype(__VA_ARGS__)>> : std::true_type {};                                     \
                                                                                                                       \
    template <class T>                                                                                                 \
    constexpr bool trait_name_##_v = trait_name_<T>::value;                                                            \
                                                                                                                       \
    template <class T>                                                                                                 \
    using require_##trait_name_ = std::enable_if_t<trait_name_<T>::value, bool>

GSE_IMPL_DEFINE_TRAIT(seed_seq,
                      std::declval<T>().generate(std::declval<std::uint32_t*>(), std::declval<std::uint32_t*>()));
// this type trait is necessary to restrict template constructors & seed function that take 'SeedSeq&& seq', otherwise
// they will get picked instead of regular seeding methods even for integer conversions. This is how standard library
// seems to do it (based on GCC implementation) so we follow their API.

// =============================
// --- Type deduction traits ---
// =============================

// We do a lot of type deduction magic on callable types,
// below are the traits for it

// Type deduction:
//    callable_type -> corresponding_signature_type
// Example:
//    Callable: 'auto lambda = [](int) -> double {}'
//    Callable  type: 'decltype(lambda)'
//    Signature type: 'double(int)'
// How it works:
//    We can deduce signature type from an 'std::function' using partial specialization.
//    By itself it doesn't work for arbitrary callable type 'Func'.
//    We can convert any callable type to 'std::function' using 'decltype(std::function(std::declval<Func>()))'.
//    Apply the trait to the result above now works for arbitrary callables.
// Limitations:
//    Function must not be ambiguous (overloaded).
template <class Func>
struct signature_of_callable_impl;

template <class R, class... Args>
struct signature_of_callable_impl<std::function<R(Args...)>> {
    using type = R(Args...);
};

template <class Func>
using signature_of_callable_t =
    typename signature_of_callable_impl<decltype(std::function(std::declval<Func>()))>::type;

// Type deduction:
//    signature_type -> return_type
// Example:
//    Signature type: 'double(int)'
//    Return    type: 'double'
template <class T>
struct return_type_of_signature_impl;

template <class R, class... Args>
struct return_type_of_signature_impl<R(Args...)> {
    using type = R;
};

template <class T>
using return_type_of_signature_t = typename return_type_of_signature_impl<T>::type;

// Type deduction:
//    callable_type -> return_type
// Example:
//    Callable: 'auto lambda = [](int) -> double {}'
//    Callable  type: 'decltype(lambda)'
//    Return    type: 'double'
template <class T>
using return_type_of_callable_t = return_type_of_signature_t<signature_of_callable_t<T>>;

// ======================
// --- SFINAE helpers ---
// ======================

template <class T>
constexpr bool always_false_v = false;

template <class T>
constexpr bool is_float_v = std::is_same_v<T, float>;

template <bool Cond>
using require = std::enable_if_t<Cond, bool>;

// template <class T, class Signature>
// using require_signature = require<std::is_convertible_v<T, std::function<Signature>>>;

template <class T, class... Args>
using require_invocable = require<std::is_invocable_v<T, Args...>>;

template <class T, class... Args>
using require_not_invocable = require<!std::is_invocable_v<T, Args...>>;

template <class Ret, class T, class... Args>
using require_invocable_r = require<std::is_invocable_r_v<Ret, T, Args...>>;

template <class T>
using require_arithmetic = require<std::is_arithmetic_v<T>>;

template <class T>
using require_floating_point = require<std::is_floating_point_v<T>>;

// --- Function signature checks ---
// ---------------------------------

// We use these in a lot of solvers so it makes sense to create shortcuts

template <class T, Extent N, class Func>
using require_vector_function = require_invocable_r<Vector<T, N>, Func, Vector<T, N>>;

template <class T, Extent N, class Func>
using require_multivariate_function = require_invocable_r<T, Func, Vector<T, N>>;

template <class T, Extent N, class Func>
using require_scalar_function = require_invocable_r<T, Func, T>;

template <class T, Extent N, class Func>
using require_time_vector_function = require_invocable_r<Vector<T, N>, Func, T, Vector<T, N>>;

// ==============================
// --- Vector-specific traits ---
// ==============================

template <class Func, class VectorType>
constexpr bool is_vector_func_v = std::is_invocable_r_v<VectorType, Func, VectorType>;

template <class Func, class VectorType>
constexpr bool is_time_vector_func_v =
    std::is_invocable_r_v<VectorType, Func, meta::value_type<VectorType>, VectorType>;

} // namespace gse::impl