[<img src ="docs/images/icon_cpp_std_17.svg">](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[<img src ="docs/images/icon_license_mit.svg">](./LICENSE.md)
[<img src ="docs/images/icon_header_only.svg">](https://en.wikipedia.org/wiki/Header-only)

# GSE (Generic Solvers for Eigen)

**GSE** is a simple Matlab-style library for solving:

- [Ordinary differential equations (ODEs)](https://en.wikipedia.org/wiki/Ordinary_differential_equation)
- [Stochastic differential equations (SDEs)](https://en.wikipedia.org/wiki/Stochastic_differential_equation)
- [Non-linear algebraic systems](https://en.wikipedia.org/wiki/Nonlinear_system)

built on top of [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page) linear algebra library.

The whole library is header-only and can be found [here](include/).

> [!Warning]
> As of now, the library is **heavily** work-in-progress, not all functionality is fully implemented.

## Design Principles

- **Header-only.** Adding the library should be as simple as single `#include`. All dependencies should be embedded.
- **Easy to use.** Library should provide a simple API similar to the ones used in [Matlab](https://en.wikipedia.org/wiki/MATLAB) / [Julia](https://en.wikipedia.org/wiki/Julia_(programming_language)) / [Numpy](https://github.com/numpy/numpy).
- **Strong type safety.** Library should use C++ type system to check as much logic as possible at compile-time and provide zero-overhead abstractions.
- **Modularity.** All solvers should be built in a modular way that allows the to be compatible with a wide range of custom systems / integrators / PRNGs / distributions / etc. This is very much unlike most Matlab & Julia solvers that tend to tightly couple integration methods to a function.

## Documentation

| Module        | Short description                        |
| ------------- | ---------------------------------------- |
| [gse::core]() | Core typedefs                            |
| [gse::alg]()  | Non-linear algebraic system solvers      |
| [gse::ode]()  | Ordinary differential equation solvers   |
| [gse::sde]()  | Stochastic differential equation solvers |

## Performance

Th library is heavily focused on performance for both small & large systems, this is achieved through following principles:

- **Dynamic & compile-time dimension support.** Similarly to Eigen [fixed-size & dynamic vectors](https://eigen.tuxfamily.org/dox/group__TopicFixedSizeVectorizable.html), all methods are templated on an optionally dynamic size, this means problems where dimension is known at compile time can operate entirely on the stack with no dynamic allocations.

- **Type-safe passing of callables.** All callables in this library are passed as SFINA-restricted template parameters, this completely avoids overhead of type-erased delegates such as `std::function` and allows compiler to properly inline callable arguments inside the solvers.

- **SIMD support.** By the virtue of using Eigen backend we can fully benefit from its [vectorization capacity](http://eigen.tuxfamily.org/index.php?title=FAQ#Vectorization) without mudding the source code with intrinsics. As of 2025 Eigen supports SSE, AVX, AVX2, AVX512, AltiVec/VSX, ARM NEON and S390x SIMD.

- **OpenMP support.** Since most of the solver logic can be expressed though Eigen's matrix operation, we also benefit from it [multi-threading support](https://eigen.tuxfamily.org/dox/TopicMultiThreading.html).

- **Fast random.** Stochastic solvers benefit greatly from using efficient [PRNGs](https://en.wikipedia.org/wiki/Pseudorandom_number_generator) and distribution implementations. Significant work on this front has been done in [utl::random](https://github.com/DmitriBogdanov/UTL/blob/master/docs/module_random.md) which is partially included here to provide fast defaults.

## See also

- [Why use Eigen as a backend]()
- [How is gse::ode different from Boost.Odeint]()
- [How to use custom scalar types]()
- [How to contribute]()

## Work in progress

- Documentation
- Non-linear algebraic API improvements
- More ODE integrators
- More SDE integrators

## Requirements

- Requires **C++17** support

## Dependencies

| Library                                      | License                                                      | Type        | Used for       | Embedded in repo |
| -------------------------------------------- | ------------------------------------------------------------ | ----------- | -------------- | ---------------- |
| [Eigen](https://eigen.tuxfamily.org)         | [MPL2](https://eigen.tuxfamily.org/index.php?title=Main_Page#License) | Third party | Linear algebra | ✔                |
| [UTL](https://github.com/DmitriBogdanov/UTL) | [MIT](https://github.com/DmitriBogdanov/UTL/blob/master/LICENSE.md) | First party | Examples       | ✔                |

## License

This project is licensed under the MIT License - see the [LICENSE.md](https://github.com/DmitriBogdanov/prototyping_utils/blob/master/LICENSE.md) for details
