# Differences between ODE libraries

[<- back to README.md](..)

## Summary

Different goals and contents. [Boost.Odeint](https://www.boost.org/doc/libs/master/libs/numeric/odeint/doc/html/index.html) & [SUNDIALS](https://computing.llnl.gov/projects/sundials) are much more suitable for heavyweight HPC tasks and are mainly focused on ODEs.

**GSE** is a convenient all-in-one package with matrices & linear algebra bundled in, useful for research work that needs quick iteration.

It also seems to be the only library (that I am aware of) to provide stochastic solvers.

## Comparison table

| Subject                                                      | GSE     | **Boost.Odeint**                | **SUNDIALS**        |
| ------------------------------------------------------------ | ------------------- | ------------------------------- | ------------------- |
| Installation                                                 | 🟢 Header-only       | 🟢 Header-only | 🟡 CMake             |
| Project scale                                                | ⚫ Medium **⁽¹⁾**    | ⚫ Large **⁽¹⁾**                 | ⚫ Huge **⁽¹⁾**      |
| Target use case | ⚫ Research | ⚫ Research & HPC | ⚫ HPC |
| API complexity                                               | 🟢 Simple **⁽¹⁾**    | 🟡 Simple-to-complex **⁽¹⁾**     | 🔴 Complex **⁽¹⁾**   |
| API flexibility                                      | 🟢 Good **⁽¹⁾**      | 🟢 Good **⁽¹⁾**                  | 🟢 Excellent **⁽¹⁾** |
| Linear algebra | 🟢 Built-in | 🔴 None | 🟢 Built-in |
| Numerical differentiation **⁽²⁾** | 🟢 Built-in | 🔴 None | ⚫ Unknown **⁽³⁾** |
| [Nonlinear](https://en.wikipedia.org/wiki/Nonlinear_system) solver selection | 🟡 Small             | 🔴 None                          | 🟢 Excellent         |
| [ODE](https://en.wikipedia.org/wiki/Ordinary_differential_equation) solver selection | 🟡 Small             | 🟢 Excellent                     | 🟢 Good              |
| [SDE](https://en.wikipedia.org/wiki/Stochastic_differential_equation) solver selection | 🟡 Small             | 🔴 None                          | 🔴 None              |
| [DAE](https://en.wikipedia.org/wiki/Differential-algebraic_system_of_equations) solver selection | 🔴 None              | 🔴 None                          | 🟢 Excellent         |
| Arbitrary precision numbers                                  | 🟢 Template | 🟢 Template              | 🔴 None              |
| SIMD compatibility                                           | 🟢 Excellent         | ⚫ Unknown **⁽³⁾**               | 🟢 Excellent         |
| OpenMP compatibility                                         | 🟢 Excellent **⁽⁴⁾** | 🟢 Good                          | ⚫ Unknown **⁽³⁾**   |
| MPI compatibility                                            | 🔴 None              | 🟢 Good                          | 🟢 Excellent         |
| GPU compatibility                                            | 🔴 None              | 🟢 Good                          | 🟢 Excellent         |

> [!Note]
>
> **(1)** Subjective metrics.
>
> **(2)** Here "numerical differentiation" refers to differentiation as an independent API, differentiation as an implementation detail is present in all libraries due to its necessity in nonlinear solvers & stiff ODEs.
>
> **(3)** Compatibility is not documented but likely present in some form.
>
> **(4)** GSE requires a bit less setup to benefit from parallelism.

