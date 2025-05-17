# Contributing to the project

[<- back to README.md](..)

## What needs to be done

ODE and SDE modules need more integration methods.

Adding new integrators is a relatively simple, but repetitive work that involves reading through literature and turning mathematical notation into code.

This is a rather long but clear process, so **contributions are extremely welcome**.

[DAE](https://en.wikipedia.org/wiki/Differential-algebraic_system_of_equations) and [boundary-value](https://en.wikipedia.org/wiki/Boundary_value_problem) modules would also be very nice to add, but currently lie outside of the planned feature scope.

## Where to find information

- A lot of conventional ODE algorithms can be found in a classic book `Ernst Hairer, Gerhard Wanner "Solving ordinary differential equations"`, [volume 1](https://link.springer.com/book/10.1007/978-3-540-78862-1) deals with non-stiff problems, [volume 2](https://link.springer.com/book/10.1007/978-3-642-05221-7) describes stiff integrators.

- SDE integration excellently covered in the book [`Peter E. Kloeden, Eckhar Platen "Numerical Solution of Stochastic Differential Equations"`](https://link.springer.com/book/10.1007/978-3-662-12616-5).

- Julia package [DifferentialEquations.jl ](https://docs.sciml.ai/DiffEqDocs/stable/solvers/ode_solve/) provides a very comprehensive listing of existing integration algorithms and describes some of their top-level properties.

## How to get started

See [`ode.hpp`](https://github.com/DmitriBogdanov/GSE/blob/master/include/GSE/ode.hpp) and [`sde.hpp`](https://github.com/DmitriBogdanov/GSE/blob/master/include/GSE/sde.hpp) headers. All integrators are listed under the `gse::XXX::integrators` section. Using existing implementations as a reference it should be relatively simple to add new methods.

Newly added integrators should be then added to a testing suite under [`tests/ode.cpp`]() or [`tests/sde.cpp`]() to check for sanity on some basic test equations. For most integrators should be as simple as adding a single template parameter to a test case. This is not a required step, but a welcome one, pull requests without testing will still be reviewed.
