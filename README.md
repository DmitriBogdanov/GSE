[<img src ="docs/images/icon_cpp_std_17.svg">](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[<img src ="docs/images/icon_license_mit.svg">](./LICENSE.md)
[<img src ="docs/images/icon_header_only.svg">](https://en.wikipedia.org/wiki/Header-only)

# GSE (Generic Solvers for Eigen)

**GSE** is a highly flexible numerical library for:

- Solving [linear systems](https://en.wikipedia.org/wiki/Linear_system)
- Solving [non-linear systems](https://en.wikipedia.org/wiki/Nonlinear_system)
- Computing numerical [derivatives](https://en.wikipedia.org/wiki/Numerical_differentiation), [gradients](https://en.wikipedia.org/wiki/Gradient) and [jacobians](https://en.wikipedia.org/wiki/Jacobian_matrix_and_determinant)
- Solving [ordinary differential equations (ODEs)](https://en.wikipedia.org/wiki/Ordinary_differential_equation)
- Solving [stochastic differential equations (SDEs)](https://en.wikipedia.org/wiki/Stochastic_differential_equation)

built on top of [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page) linear algebra functionality.

The whole library is header-only and can be found [here](include/).

> [!Warning]
> As of now, the library is in **early alpha**, it contains core functionality, but there is plenty of work left to be done in both implementation and its documentation.

## Design Principles

- **Header-only.** Adding the library should be as simple as a single `#include`. All dependencies should be embedded.
- **Easy to use.** Library should provide a simple APIs similar to the ones used in [Matlab](https://en.wikipedia.org/wiki/MATLAB) / [Julia](https://en.wikipedia.org/wiki/Julia_(programming_language)) / [Numpy](https://github.com/numpy/numpy).
- **Strong type safety.** Library should utilize C++ type system to check as much logic as possible at compile-time and provide zero-overhead abstractions.
- **Extreme modularity.** All things should be built in a generic modular way so different methods can be [spliced together seamlessly "like Lego bricks"](./docs/guide_modularity_showcase.md). This is very much unlike most Matlab & Julia solvers that tend to tightly couple methods to a function.

## Documentation

| Module                                             | Short description                        |
| -------------------------------------------------- | ---------------------------------------- |
| [**gse::core**](./docs/module_core.md)             | Core typedefs                            |
| [**gse::linear**](./docs/module_linear.md)         | Linear system solvers                    |
| [**gse::nonlinear**](./docs/module_nonlinear.md)   | Non-linear system solvers                |
| [**gse::derivative**](./docs/module_derivative.md) | Numerical derivative computation         |
| [**gse::gradient**](./docs/module_gradient.md)     | Numerical gradient computation           |
| [**gse::jacobian**](./docs/module_jacobian.md)     | Numerical Jacobian computation           |
| [**gse::ode**](./docs/module_ode.md)               | Ordinary differential equation solvers   |
| [**gse::sde**](./docs/module_sde.md)               | Stochastic differential equation solvers |

## Performance

The library aims to provide good performance for both small & large systems, this is achieved through following principles:

- **Dynamic & compile-time dimension support.** Similarly to Eigen [fixed-size & dynamic vectors](https://eigen.tuxfamily.org/dox/group__TopicFixedSizeVectorizable.html), all methods are templated on an optionally dynamic size, this means problems where dimension is known at compile time can operate entirely on the stack with no dynamic allocations.

- **Type-safe passing of callables.** All callables in this library are propagated as perfectly-forwarded SFINA-restricted template parameters, this completely avoids the overhead of type-erased delegates such as `std::function` and allows compiler to properly inline methods inside the solvers.

- **SIMD support.** By the virtue of using Eigen backend we can fully benefit from its [vectorization capacity](http://eigen.tuxfamily.org/index.php?title=FAQ#Vectorization) without mudding the source code with intrinsics. As of 2025 Eigen supports SSE, AVX, AVX2, AVX512, AltiVec/VSX, ARM NEON and S390x SIMD.

- **Multi-threading support.** Since most of the solver logic can be expressed though Eigen's matrix operations, we also benefit from its [multi-threading support](https://eigen.tuxfamily.org/dox/TopicMultiThreading.html) based on [OpenMP](https://en.wikipedia.org/wiki/OpenMP).

- **Fast random.** Stochastic solvers benefit greatly from using efficient [PRNGs](https://en.wikipedia.org/wiki/Pseudorandom_number_generator) and distribution implementations. Significant work on this front has been done in [utl::random](https://github.com/DmitriBogdanov/UTL/blob/master/docs/module_random.md) which is partially included here to provide fast RNG defaults. This aspect of modeling is frequently neglected.

## See also

- [Why use Eigen as a backend](./docs/guide_backend_motivation.md)
- [How is GSE different from Boost.Odeint and SUNDIALS](./docs/guide_library_differences.md)
- [How to contribute](./CONTRIBUTING.md)

## Work in progress

- Documentation
- Figure out a good solution for Eigen pass-by-value issues
- Integrate solvers with Eigen expression template base classes
- Modules: `gse::curvature`, `gse::hessian`, `gse::scalar_nonlinear`, `gse::optimization`, `gse::scalar_optimization`, `gse::bracketing`, `gse::scalar_bracketing`
- `gse::nonlinear` API improvements
- Stiff ODE methods
- High order SDE methods
- More derivative/gradient/jacobian differentiation methods
- Stable nonlinear iterative methods
- Support for auto-differentiation **(?)**
- Platform-independent distributions for stochastic modeling
- PRNG API improvements
- CMake cleanup

## Requirements

- Requires **C++17** support

## Dependencies

| Link                                               | License                                                      | Type        | Used for       | Part of the library | Embedded in repo |
| -------------------------------------------------- | ------------------------------------------------------------ | ----------- | -------------- | ------------------- | ---------------- |
| [Eigen](https://eigen.tuxfamily.org)               | [MPL2](https://eigen.tuxfamily.org/index.php?title=Main_Page#License) | Third party | Linear algebra | ✔                   | ✔                |
| [doctest](https://github.com/doctest/doctest)      | [MIT](https://github.com/doctest/doctest/blob/master/LICENSE.txt) | Third party | Unit testing   | ✘                   | ✔                |
| [nanobench](https://github.com/martinus/nanobench) | [MIT](https://github.com/martinus/nanobench/blob/master/LICENSE) | Third party | Benchmarking   | ✘                   | ✔                |
| [UTL](https://github.com/DmitriBogdanov/UTL)       | [MIT](https://github.com/DmitriBogdanov/UTL/blob/master/LICENSE.md) | First party | Examples       | ✘                   | ✔                |

## License

This project is licensed under the MIT License - see the [LICENSE.md](https://github.com/DmitriBogdanov/prototyping_utils/blob/master/LICENSE.md) for details
