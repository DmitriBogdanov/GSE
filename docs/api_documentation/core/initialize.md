# Initialize

<div class="grid cards" markdown>

- Implementation
- [`#include "GSE/core/initialize.hpp"`](../../../include/GSE/core/initialize.hpp)
- Namespace
- `gse::init::`
- Content
- Matrix initialization functions

</div>

## Definitions

```cpp
// Zero matrices
template <scalar T = double, Extent N = dynamic>
Vector<T, N> zero(Idx rows);

template <scalar T = double, Extent N = dynamic, Extent M = dynamic>
Matrix<T, N, M> zero(Idx rows, Idx cols);

// Constant matrices
template <scalar T = double, Extent N = dynamic, Extent M = dynamic>
Matrix<T, N, M> constant(Idx rows, Idx cols, T value);

template <scalar T = double, Extent N = dynamic, Extent M = dynamic>
Matrix<T, N, M> constant(Idx rows, Idx cols, T value);

// Random matrices
template <
    scalar                  T    = double,
    Extent                  N    = dynamic,
    random_distribution     Dist = UniformDistribution<T>,
    random_number_generator Gen  = PRNG
>
Vector<T, N> random(Idx rows, Dist&& dist = Dist{}, Gen&& gen = Gen{});

template <
    scalar                  T    = double,
    Extent                  N    = dynamic,
    random_distribution     Dist = UniformDistribution<T>,
    random_number_generator Gen  = PRNG
>
Matrix<T, N, M> random(Idx rows, Idx cols, Dist&& dist = Dist{}, Gen&& gen = Gen{});

// Other matrices
template <scalar T = double, Extent N = dynamic>
[[nodiscard]] Vector<T, N> linspace(Idx rows, T min, T max);

template <scalar T = double, Extent N = dynamic, Extent M = dynamic>
[[nodiscard]] Matrix<T, N, M> identity(Idx rows, Idx cols);
```

> [!Note]
> When used with static extents `N` / `M` functions `zero()`, `random()` and `identity()` also provide default values for `rows` / `cols` parameters. See the [examples](../../usage_examples/static_and_dynamic_matrices.md). Should the `rows` / `cols` be passed explicitly (which usually happens in generic code) they will be asserted to match the compile-time extents.

## API

### Zero matrices

> ```cpp
> template <scalar T = double, Extent N = dynamic>
> Vector<T, N> zero(Idx rows);
> ```

Creates a zero-filled `Vector<T, N>` with `rows` elements.

**Note:** For static `N`, parameter `rows` is optional.

> ```cpp
> template <scalar T = double, Extent N = dynamic, Extent M = dynamic>
> Matrix<T, N, M> zero(Idx rows, Idx cols);
> ```

Creates a zero-filled `Matrix<T, N, M>` with `rows` x `cols` elements.

**Note:** For static `N` / `M`, parameters `rows` / `cols` are optional.

### Constant matrices

> ```cpp
> template <scalar T = double, Extent N = dynamic, Extent M = dynamic>
> Matrix<T, N, M> constant(Idx rows, Idx cols, T value);
> ```

Creates a `Vector<T, N>` with `rows` elements filled with `value`.

**Note:** For static `N`, parameter `rows` should correspond to the static value.

> ```cpp
> template <scalar T = double, Extent N = dynamic, Extent M = dynamic>
> Matrix<T, N, M> constant(Idx rows, Idx cols, T value);
> ```

Creates a `Matrix<T, N, M>` with `rows` x `cols` elements filled with `value`.

**Note:** For static `N` / `M`, parameters `rows` / `cols` should correspond to the static value.

### Random matrices

> ```cpp
> template <
>     scalar                  T    = double,
>     Extent                  N    = dynamic,
>     random_distribution     Dist = UniformDistribution<T>,
>     random_number_generator Gen  = PRNG
> >
> Vector<T, N> random(Idx rows, Dist&& dist = Dist{}, Gen&& gen = Gen{});
> ```

Creates a `Vector<T, N>` with `rows` elements filled with random values from `dist` generated with `gen`.

**Note:** For static `N`, parameter `rows` is optional.

> ```cpp
> template <
>     scalar                  T    = double,
>     Extent                  N    = dynamic,
>     random_distribution     Dist = UniformDistribution<T>,
>     random_number_generator Gen  = PRNG
> >
> Matrix<T, N, M> random(Idx rows, Idx cols, Dist&& dist = Dist{}, Gen&& gen = Gen{});
> ```

Creates a `Matrix<T, N, M>` with `rows` x `cols` elements filled with random values from `dist` generated with `gen`.

**Note:** For static `N` / `M`, parameters `rows` / `cols` are optional.

### Other matrices

> ```cpp
> template <scalar T = double, Extent N = dynamic>
> [[nodiscard]] Vector<T, N> linspace(Idx rows, T min, T max);
> ```

Creates a `Vector<T, N>` with `rows` elements filled with linearly spaced values from `min` to `max`.

**Note:** For static `N`, parameter `rows` should correspond to the static value.

> ```cpp
> template <scalar T = double, Extent N = dynamic, Extent M = dynamic>
> [[nodiscard]] Matrix<T, N, M> identity(Idx rows, Idx cols);
> ```

Creates an identity `Matrix<T, N, M>` with `rows` x `cols` elements.

**Note:** For static `N` / `M`, parameters `rows` / `cols` are optional.

## Examples

TODO:

## Additional notes

`Eigen` matrices provide similar construction methods as static member functions `::Zero()`, `::Constant()`, `::Random()` and `::LinSpaced()`. The reason we re-implement & wrap this functionality is to present a uniformly styled API and add some additional customization points (such as distributions and random number generation in `gse::init::random()`).