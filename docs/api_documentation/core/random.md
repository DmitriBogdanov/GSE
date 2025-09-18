# Random

<div class="grid cards" markdown>

- Implementation
- [`#include "GSE/core/random.hpp"`](../../../include/GSE/core/random.hpp)
- Namespace
- `gse::`
- Content
- Platform-independent PRNG and random distributions

</div>

## Definitions

```cpp
// Concepts
template <class Seq > concept seeding_sequence;
template <class Gen > concept random_number_generator;
template <class Dist> concept random_distribution;
template <class T   > concept arithmetic;

// Generator
struct PRNG {
    using result_type = std::uint64_t;
    
    constexpr explicit PRNG(result_type seed = /* default seed */) noexcept;
    constexpr explicit PRNG(seeding_sequence auto&& seq);
    
    constexpr void seed(result_type seed) noexcept;
    constexpr void seed(seeding_sequence auto&& seq);
    
    static constexpr result_type min() noexcept;
    static constexpr result_type max() noexcept;
    
    constexpr result_type operator()() noexcept
};

// Distributions
template <std::integral T = int>
struct UniformIntDistribution { };

template <std::floating_point T = double>
struct UniformRealDistribution { };

template <std::floating_point T = double>
struct NormalDistribution { };

// Convenience
template <arithmetic T>
using UniformDistribution = std::conditional_t<
    std::integral<T>, UniformIntDistribution<T>, UniformRealDistribution<T>
>;
```

## API

### Concepts

### Generator

### Distributions

### Convenience

`text`