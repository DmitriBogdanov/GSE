# Using custom scalar types

[<- back to README.md](..)

## Switching precision

Scalar precision can be switched globally by defining a policy:

```cpp
#define GSE_USE_SCALAR_TYPE float
#include "GSE/core.hpp" // now uses single-precision for 'Scalar', 'Vector' and 'Matrix'
```

This option supports any float-like type, including `float`,  `long double`, `__float128` extension, C++23 `std::float128_t` and arbitrary precision floats provided by other libraries.

## Design motivation

The reason for using a global policy design instead of a more localized template approach is that we can provide significantly cleaner API for the common use at the cost of making novel uses less convenient. Realistically, arbitrary precision floats are rarely necessary and single-precision is usually motivated by GPU computation which isn't a focus of this library.
