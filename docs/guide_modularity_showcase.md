# Modularity showcase

[<- back to README.md](..)

The library's architecture is **built around the idea of treating all numerical methods like a constructor** — complex numerical methods are built from simpler methods like building blocks.

**For example, let's say we want to define a custom ODE integrator** which should:

1. Use symplectic Euler's scheme (an implicit method)
2. Solve underlying nonlinear systems using Newton's method
3. Compute jacobians for Newton's method using central difference formula
4. Solve linear systems inside Newton's method using QR decomposition

Defining such integrator is as simple as composing it from corresponding templates:

```cpp
using custom_ode_method = gse::ode::method::SymplecticEuler<double,
    gse::nonlinear::method::Newton<double,
        gse::jacobian::method::CentralDifference<double>,
        gse::linear::method::HouseholderQR
    >
>;
```

In the same fashion we can configure every single parameter of all "building blocks":

```cpp
custom_ode_method method;

method.time_step                                  = 1e-3; // ODE time step
method.nonlinear_method.precision                 = 1e-6; // internal Newton's method precision
method.nonlinear_method.jacobian_method.diff_step = 1e-5; // internal jacobian numerical diff. step
// ...
```

and then we can just throw this method into the `solve()`:

```cpp
gse::ode::solve(f, x0, t0, t1, callback, method);
```

This approach applies to **all** methods in the library, making it extremely easy switch implementation details at every level.

> [!Note]
> For most purposes building entire methods like this is not necessary, the API has appropriate defaults at every level and chooses the underlying methods automatically. Module documentation contains examples of how common use cases tend to look.

## Why would we need such flexibility

Modularity comes into play when we want to configure something due to our knowledge of the problem, for example:

- If we know system jacobian changes relatively smoothly, we might want to select a faster differentiation method
- If we're dealing with a particularly stiff system, we might want to more expensive, but numerically stable methods for internal steps
- If we know some internal matrix is going to be positive-definite, we can use a faster linear solver for it (like [Cholesky LLT](https://en.wikipedia.org/wiki/Cholesky_decomposition))
- If we're using a 3rd party library that provides custom random numbers and distributions, we might want to use them instead of standard ones when solving SDEs
- If this library doesn't contain some specific method that we need, it is trivially easy to implement it as a stand-alone building block and insert it into a general solver

