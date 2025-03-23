[![Language](https://img.shields.io/badge/C++-std=17-blue.svg?style=flat&logo=cplusplus)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/DmitriBogdanov/GSE/blob/master/LICENSE.md)

# GSE (Generic Solver for Eigen)

This library provides Matlab-style methods for solving:

- [Ordinary differential equations (ODE)](https://en.wikipedia.org/wiki/Ordinary_differential_equation)
- [Stochastic differential equations (SDE)](https://en.wikipedia.org/wiki/Stochastic_differential_equation)
- [Non-linear algebraic systems](https://en.wikipedia.org/wiki/Nonlinear_system)

built on top of [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page) linear algebra library.

Entire library is header-only and can be found [here](include/).

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
| [Eigen](https://eigen.tuxfamily.org) | [MPL2](https://eigen.tuxfamily.org/index.php?title=Main_Page#License) | linear algebra | ✔                |

- [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page) linear algebra library

## License

This project is licensed under the MIT License - see the [LICENSE.md](https://github.com/DmitriBogdanov/prototyping_utils/blob/master/LICENSE.md) for details
