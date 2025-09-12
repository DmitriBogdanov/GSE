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
using Idx    = std::ptrdiff_t; // (1)!
using Extent = int;
using Uint   = unsigned int;

constexpr Extent dynamic = Eigen::Dynamic;

// Matrix types
template <class T = double, Extent N = dynamic>
using Vector = Eigen::Vector<T, N>;

template <class T = double, Extent M = dynamic, Extent N = dynamic>
using Matrix = Eigen::Matrix<T, N, M>;
```

1. This type is index.

## API

```cpp
using Idx = std::ptrdiff_t;
```

Index type used by GSE matrices.

```cpp
using Extent = int;
```

Extent type used by GSE matrices.

```cpp
using Uint   = unsigned int;
```

Unsigned type used for positive counters.
