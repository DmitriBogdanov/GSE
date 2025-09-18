# Types

<div class="grid cards" markdown>

- Implementation
- [`#include "GSE/core/types.hpp"`](../../../include/GSE/core/types.hpp)
- Namespace
- `gse::`
- Content
- Core typedefs of the library

</div>

## Definitions

```cpp
// Indexation
using Idx    = std::ptrdiff_t;
using Extent = int;
using Uint   = unsigned int;

constexpr Extent dynamic = Eigen::Dynamic;

// Matrix types
template <class T = double, Extent M = dynamic>
using Vector = Eigen::Vector<T, M>;

template <class T = double, Extent M = dynamic, Extent N = dynamic>
using Matrix = Eigen::Matrix<T, M, N>;
```

## API

### Indexation

> ```cpp
> using Idx = std::ptrdiff_t;
> ```

Index type used by GSE matrices.

> ```cpp
> using Extent = int;
> ```

Extent type used by GSE matrices.

> ```cpp
> using Uint   = unsigned int;
> ```

Unsigned type used for positive counters.

> ```cpp
> constexpr Extent dynamic = Eigen::Dynamic;
> ```

Value specifying that corresponding matrix extent should be dynamic.

### Matrix types

> ```cpp
> template <class T = double, Extent M = dynamic>
> using Vector = Eigen::Vector<T, M>;
> ```

Row-major column vector with scalar type `T` and row extent `M`.

> ```cpp
> template <class T = double, Extent M = dynamic, Extent N = dynamic>
> using Matrix = Eigen::Matrix<T, M, N>;
> ```

Row-major matrix with scalar type `T`, row extent `M` and column extent `N`.