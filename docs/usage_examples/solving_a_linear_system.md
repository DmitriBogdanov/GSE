# Solving a linear system

## Linear systems

Linear system $A x = b$ is defined by a **square non-degenerate** matrix $A$ and its RHS vector $b$.

System solution is a vector $x$ for which equality above holds.

Linear solvers provided by this library are simply thin wrappers around the solvers implemented in Eigen, made mostly for the purpose of API uniformity. See corresponding [API documentation]() for a listing of methods.

## Solving a linear system

Let's solve a $30 \times 30$ linear system $A x = b$ with randomly generated matrices:

[ [Run this code]() ] [ [Open source file]() ]

```cpp
gse::Matrix<> A = gse::init::random(30, 30);
gse::Vector<> b = gse::init::random(30);

gse::Vector<> x = gse::linear::solve(A, b);
```

## Customizing the method

Let's solve a $10 \times 10$ linear system $A x = b$ with randomly generated matrices using QR decomposition method:

[ [Run this code]() ] [ [Open source file]() ]

```cpp
gse::Matrix<double, 10, 10> A = gse::init::random<double, 10, 10>();
gse::Vector<double, 10    > b = gse::init::random<double, 10    >();

gse::Vector<double, 10> x = gse::linear::solve(A, b, gse::linear::methods::QR{});
```
