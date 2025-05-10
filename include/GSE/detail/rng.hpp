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
#include <cstdint> // uint32_t, uint64_t
#include <limits>  // numeric_limits<>::max()

#include "traits.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Better default RNG that the one used in Eigen (better quality & performance), more explicit global PRNG 

// ____________________ IMPLEMENTATION ____________________

// ===========================
// --- PRNG implementation ---
// ===========================

namespace gse::impl::rng {

constexpr std::uint64_t default_seed = std::numeric_limits<std::uint64_t>::max() / 2 + 1;

// Implementation of SplitMix64 PRNG from the first-party 'utl::random' library,
// see https://github.com/DmitriBogdanov/UTL/blob/master/include/UTL/random.hpp
//
// One of the fastest statistically solid PRNGs, we use it instead of Eigen's internal LCG.
// If more RNG stuff is needed see the original 'utl::random' lib, GSE API are quite flexible
// and will be able to make use of its PRNGs & distributions. This one is embedded simply to
// ensure a solid default choice.
//
class SplitMix64 {
public:
    using result_type = std::uint64_t;

private:
    result_type s{};

public:
    constexpr explicit SplitMix64(result_type seed = default_seed) noexcept { this->seed(seed); }

    template <class SeedSeq, require_seed_seq<SeedSeq> = true>
    explicit SplitMix64(SeedSeq&& seq) {
        this->seed(seq);
    }

    [[nodiscard]] static constexpr result_type min() noexcept { return 0; }
    [[nodiscard]] static constexpr result_type max() noexcept { return std::numeric_limits<result_type>::max(); }

    constexpr void seed(result_type seed) noexcept { this->s = seed; }

    template <class SeedSeq, require_seed_seq<SeedSeq> = true>
    void seed(SeedSeq&& seq) {
        std::array<std::uint32_t, 2> temp;
        seq.generate(temp.begin(), temp.end());
        this->s = static_cast<std::uint64_t>(temp[0]) | (static_cast<std::uint64_t>(temp[1]) << 32);
    }

    constexpr result_type operator()() noexcept {
        std::uint64_t result = (this->s += 0x9E3779B97f4A7C15);
        result               = (result ^ (result >> 30)) * 0xBF58476D1CE4E5B9;
        result               = (result ^ (result >> 27)) * 0x94D049BB133111EB;
        return result ^ (result >> 31);
    }
};

} // namespace gse::impl::rng

// ================
// --- PRNG API ---
// ================

namespace gse::rng {

using PRNG = impl::rng::SplitMix64;

inline PRNG default_generator;

inline void seed(std::uint64_t random_seed) noexcept { default_generator.seed(random_seed); }

} // namespace gse::rng
