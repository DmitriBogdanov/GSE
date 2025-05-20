# Why use Eigen as a backend

[<- back to README.md](..)

Most ODE, SDE and algebraic solvers are algorithms written in terms of matrix operations and linear systems.

**1.** By using Eigen as a backend **we can benefit from a multitude of its powerful features**:

- SIMD compatibility
- Multi-threading
- Expression templates
- Efficient linear algebra routines
- Arbitrary scalar type support

**2.** **Matrix operations, linear algebra and non-linear solvers go well together.** Since non-linear solvers need matrix operations anyway, it makes sense to bundle them with a mature matrix library rather than reinvent the wheel, especially considering the scale and complexity of efficient linear algebra routines. This also helps users define their systems in terms of proper matrix operations.

**3.** By using high-level matrix API we can **keep algorithm implementations relatively simple and "close to the textbook"**, which is particularly useful for using them as a reference.
