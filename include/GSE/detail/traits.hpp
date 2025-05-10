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
#include <type_traits> // is_invocable<>, is_invocable_r<>, is_convertible<>
#include <utility>     // declval<>()

// ____________________ DEVELOPER DOCS ____________________

// Type traits & SFINAE helpers

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
// they will get pick instead of regular seeding methods for even for integer conversions. This is how standard library
// seems to do it (based on GCC implementation) so we follow their API.

// ======================
// --- SFINAE helpers ---
// ======================

template <bool Cond>
using require = std::enable_if_t<Cond, bool>;

template <class T, class Signature>
using require_signature = require<std::is_convertible_v<T, std::function<Signature>>>;

template <class T, class... Args>
using require_invocable = require<std::is_invocable_v<T, Args...>>;

template <class T, class... Args>
using require_not_invocable = require<!std::is_invocable_v<T, Args...>>;

template <class Ret, class T, class... Args>
using require_invocable_r = require<std::is_invocable_r_v<Ret, T, Args...>>;

} // namespace gse::impl