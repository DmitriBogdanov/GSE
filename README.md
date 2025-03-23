[![Language](https://img.shields.io/badge/C++-std=17-blue.svg?style=flat&logo=cplusplus)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/DmitriBogdanov/GSE/blob/master/LICENSE.md)

# GSE (Generic Solver for Eigen)

This library provides Matlab-style methods for solving:

- [Ordinary differential equations (ODE)](https://en.wikipedia.org/wiki/Ordinary_differential_equation)
- [Stochastic differential equations (SDE)](https://en.wikipedia.org/wiki/Stochastic_differential_equation)
- [Non-linear algebraic systems](https://en.wikipedia.org/wiki/Nonlinear_system)

built on top of [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page) linear algebra library.

Entire library is header-only and can be found [here](include/).

## Desing Principles

- **Easy to include.** No build system is required by the library, adding it should be as simple as single `#include`. All dependencies are embedded.

- **Total modularity.** All solvers are built in a completely modular way, allowing them to seamlessly work with any custom system / integrator / PRNG / distribution / etc. provided, this is very much unlike most Matlab & Julia solvers that tightly couple methods to a function & API.

## Performance

This library is heavily focused on performance, this is achieved through following principles:

- **Support of dynamic & compile-time dimensions.** Similarly to Eigen [fixed-size & dynamic vectors](https://eigen.tuxfamily.org/dox/group__TopicFixedSizeVectorizable.html), all methods are templated on an optionally dynamic size, this means problems where dimension is known at compile time can operate entirely on the stack with no dynamic allocations. This also opens up room for additional compiler optimizations such as SIMD / unrolling / omitted bound checking and etc.

- **Type-safe passing of callables.** All callables in this library are passed as SFINA-restricted template parameters, this completely avoids overhead of type-erased delegates such as `std::function` and allows compiler to properly inline callables arguments inside the functions they were passed in.

- **Allocation reuse.** This library makes heavy use of stateful functors to avoid vector & matrix reallocation that happens with most "naively" written math functions & integrators.

## Documentation

TODO:

## Work in progress

- Documentation
- Non-linear algebraic systems
- Implicit methods
- Better adaptive ODE integrators
- Better SDE integrators

## Requirements

- Requires **C++17** support

## Dependencies

| Library                              | License                                                               | Used for       | Embedded in repo |
| ------------------------------------ | --------------------------------------------------------------------- | -------------- | ---------------- |
| [Eigen](https://eigen.tuxfamily.org) | [MPL2](https://eigen.tuxfamily.org/index.php?title=Main_Page#License) | Linear algebra | ✔                |

## License

This project is licensed under the MIT License - see the [LICENSE.md](https://github.com/DmitriBogdanov/prototyping_utils/blob/master/LICENSE.md) for details
