# Modularity showcase

[<- back to README.md](..)

The library's architecture is built around the idea of treating all numerical methods like a constructor — complex numerical methods are built from simpler methods like building blocks.

For example, let's say we want to define a custom ODE integrator which should:

1. Use symplectic Euler's scheme (an implicit method)
2. Solve underlying nonlinear systems using Newton's method
3. Compute jacobians for Newton's method using central difference formula
4. Solve linear systems inside Newton's method using full-pivot LU decomposition

Defining such integrator is as simple as composing it from corresponding templates:

```cpp
using ode_integrator = ode::method::SymplecticEuler<
    double,
    nonlinear::method::Newton<
        jacobian::method::CentralDifference,
        linear::method::FullPivotLU
    >
>;
```

This approach applies to **all** methods in the library, making it extremely easy switch implementation details at every level.

> [!Note]
> For most purposes building entire methods like this is not necessary, the API has appropriate defaults at every level and chooses the underlying methods automatically. Module documentation contains examples of how common use cases tend to look.

## Why would we need such flexibility

Modularity comes into play when we want to configure something due to our knowledge of the problem, for example:

- If we know system jacobian changes relatively smooth, we might want to select a faster differentiation method
- When we're dealing with a particularly stiff system, we might want to use a more numerically stable linear solver
- If we're using a 3rd party library that provides custom random numbers and distributions, we might want to use them instead of standard ones
- If this library doesn't contain some specific method that we need, it is trivially easy to implement it as a compatible building block and insert it into a general solver
