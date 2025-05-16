# Differences between ODE libraries

| Subject                                                      | **gse::ode**  | **Boost.Odeint**                | **SUNDIALS** |
| ------------------------------------------------------------ | ------------- | ------------------------------- | ------------ |
| Installation                                                 | 🟢 Header-only | 🟡 Header-only with include path | 🔴 CMake      |
| Project scale                                                | ⚫ Medium      | ⚫ Large                         | ⚫ Huge       |
| API complexity**⁽¹⁾**                                        | 🟢 Simple      | 🟡 Simple-to-complex             | 🔴 Complex    |
| API flexibility**⁽¹⁾**                                       | 🟢 Good        | 🟢 Good                          | 🟢 Excellent  |
| [ODE](https://en.wikipedia.org/wiki/Ordinary_differential_equation) solver selection | 🟡 Small       | 🟢 Excellent                     | 🟢 Good       |
| [SDE](https://en.wikipedia.org/wiki/Stochastic_differential_equation) solver selection | 🟡 Small       | 🔴 None                          | 🔴 None       |
| [DAE](https://en.wikipedia.org/wiki/Differential-algebraic_system_of_equations) solver selection | 🔴 None        | 🔴 None                          | 🟢 Excellent  |
| [Algebraic](https://en.wikipedia.org/wiki/Nonlinear_system) solver selection | 🟡 Small       | 🔴 None                          | 🟢 Excellent  |
| Complex numbers                                              | 🟡 ?           | 🟡 ?                             | 🔴 None       |
| Arbitrary precision numbers                                  | 🟡 ?           | 🟡 ?                             | 🔴 None       |
| Autodifferentiation                                          | 🔴 None        | 🔴 None                          | 🔴 None       |
| SIMD compatibility                                           | 🟢 Good        | ⚫ Unknown                       | 🟢 Excellent  |
| OpenMP compatibility                                         | 🟢 Excellent   | 🟢 Good                          | ⚫ Unknown    |
| MPI compatibility                                            | 🔴 None        | 🟢 Good                          | 🟢 Excellent  |
| GPU compatibility                                            | 🔴 None        | 🟢 Good                          | 🟢 Excellent  |

> [!Note]
>
> **(1)** Subjective metrics.

**Summary:** Odeint & SUNDIALS are much more suitable for heavyweight HPC tasks. GSE is a convenient all-in-one package with matrices & linear algebra bundled in, useful for research work that needs quick iteration.
