# Concepts

<div class="grid cards" markdown>

- Implementation
- [`#include "GSE/core/concepts.hpp"`](../../../include/GSE/core/concepts.hpp)
- Namespace
- `gse::`
- Content
- Core concepts of the library

</div>

## Definitions

```cpp
// Generic constraints
template <class From, class To>
concept convertible_to;

template <class Func, class R, class... Args>
concept invocable_r;

// Type constraints
template <class T>
concept scalar;

template <class Vec>
concept vector;

template <class Mat>
concept matrix;

// Size constraints
template <class Mat, Extent M>
concept rows_equal_to;

template <class Mat, Extent N>
concept cols_equal_to;

// Function constraints
template <class Func, class T>
concept scalar_function;

template <class Func, class T, Extent N>
concept vector_function;

template <class Func, class T, Extent N>
concept multivariate_function;
```

## API

### Generic constraints

```cpp
template <class From, class To>
concept convertible_to;
```

Concept that constrains `From` to being implicitly convertible to `To`.

Unlike [`std::convertible_to`](https://en.cppreference.com/w/cpp/concepts/convertible_to.html), this concept doesn't allow matrix conversions that would lead to a mismatch of their static sizes. For non-matrix types `gse::` version functions the same as the standard concept.

**Examples:**

| `From`                     | `To`                       | `std::convertible_to<From, To>` | `gse::convertible_to<From, To>` |
| -------------------------- | -------------------------- | ------------------------------- | ------------------------------- |
| `Matrix<3, 5>`             | `Matrix<3, 5>`             | `true`                          | `true`                          |
| `Matrix<dynamic, dynamic>` | `Matrix<3, 5>`             | `true`                          | `true`                          |
| `Matrix<3, 5>`             | `Matrix<dynamic, dynamic>` | `true`                          | `true`                          |
| `Matrix<3, 5>`             | `Matrix<4, 5>`             | `true`                          | `false`                         |
| `Matrix<3, 5>`             | `Matrix<2, 2>`             | `true`                          | `false`                         |

```cpp
template <class Func, class R, class... Args>
concept invocable_r;
```

Concept that constrains `Func` to being a callable with signature `R ( Args... )`.

**Note:** Similarly to `convertible_to<>`, this concept can catch dimension mismatch in the return value.

### Type constraints

```cpp
template <class T>
concept scalar;
```

Concept that constrains `T` to the allowed scalar types.

```cpp
template <class Vec>
concept vector;
```

Concept that constrains `Vec` to the allowed vector types.

```cpp
template <class Mat>
concept matrix;
```

Concept that constrains `Mat` to the allowed matrix types.

### Size constraints

```cpp
template <class Mat, Extent M>
concept rows_equal_to;
```

Concept that constrains the rows extent of a matrix `Mat` to `M`.

```cpp
template <class Mat, Extent N>
concept cols_equal_to;
```

Concept that constrains the cols extent of a matrix `Mat` to `N`.

### Function constraints

```cpp
template <class Func, class T>
concept scalar_function;
```

Concept that constrains `Func` to a signature of a scalar function: `T ( T )`.

```cpp
template <class Func, class T, Extent N>
concept vector_function;
```

Concept that constrains `Func` to a signature of a vector function: `Vector<T, N> ( Vector<T, N> )`.

```cpp
template <class Func, class T, Extent N>
concept multivariate_function;
```

Concept that constrains `Func` to a signature of a multivariate function: `T ( Vector<T, N> )`.