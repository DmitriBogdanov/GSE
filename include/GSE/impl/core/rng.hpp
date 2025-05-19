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
#include <random>  // uniform_real_distribution<>, normal_distribution<>

#include "./traits.hpp"

// ____________________ DEVELOPER DOCS ____________________

// Better default PRNG, Eigen uses 'rand()' which is quite subpar both in speed and quality,
// here we implement SplitMix64 which is one of the fastest statistically solid 64-bit PRNGs.
//
// Implementation taken from the first-party library 'utl::random',
// see https://github.com/DmitriBogdanov/UTL/blob/master/include/UTL/random.hpp
//
// If more RNG stuff is needed see the original 'utl::random' lib, GSE API are quite flexible
// and will be able to make use of its PRNGs & distributions. This one is embedded simply to
// ensure a solid default choice.

// ____________________ IMPLEMENTATION ____________________

// =======================
// --- SplitMix64 PRNG ---
// =======================

namespace gse::impl {

class SplitMix64 {
public:
    using result_type = std::uint64_t;

private:
    result_type s{};

    constexpr static std::uint64_t default_seed = std::numeric_limits<std::uint64_t>::max() / 2 + 1;

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

// --- Type alias ---
// ------------------

using PRNG = SplitMix64;

template <class T>
using UniformDistribution = std::uniform_real_distribution<T>;

template <class T>
using NormalDistribution = std::normal_distribution<T>;

} // namespace gse::impl
