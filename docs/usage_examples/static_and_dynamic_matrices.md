# Static & dynamic matrices

## Matrix types

GSE matrices work as simple typedefs for matrices from Eigen:

```cpp
template <class T = double, Extent N = dynamic>
using Vector = Eigen::Vector<T, N>;

template <class T = double, Extent M = dynamic, Extent N = dynamic>
using Matrix = Eigen::Matrix<T, N, M>;
```

As such, they have a notion of **scalar type** and **extents**:

- Scalar type `T` specifies the type of matrix coefficients.
- Extents `M`, `N`, specify dimensions of the matrix which are known at compile time. Dimensions without compile time value are set to `gse::dynamic`. Using fixed matrices is usually more performant for small problems as it allows Eigen to avoid dynamic memory allocation and unroll loops. Internally, fixed-size matrices are stored as plain arrays.

> [!Tip]
> Refer to the [original Eigen documentation]((https://libeigen.gitlab.io/eigen/docs-nightly/group__TutorialMatrixClass.html)) for more details on how to use matrices.

> [!Tip]
> By default GSE uses `double` precision and `dynamic` extents for all its templates as it is the most common use case.

## Dynamically sized matrices

Let's compute $A^T b + c$ with randomly generated dynamic matrices and print the result:

[ [Run this code]() ] [ [Open source file]() ]

```cpp
gse::Matrix<> A = gse::init::random(30, 30);
gse::Vector<> b = gse::init::random(30);
gse::Matrix<> c = gse::init::random(30);

gse::Vector<> result = A.transpose() * b + c;

for (gse::Idx i = 0; i < 30; ++i) std::cout << result(i) << '\n';
```

## Statically sized matrices

Let's compute $A^T b + c$ with randomly generated static single-precision matrices and print the result:

[ [Run this code]() ] [ [Open source file]() ]

```cpp
gse::Matrix<float, 3, 3> A = gse::init::random<float, 3, 3>();
gse::Vector<float, 3   > b = gse::init::random<float, 3   >();
gse::Vector<float, 3   > c = gse::init::random<float, 3   >();

gse::Vector<float, 3> result = A.transpose() * b + c;

for (gse::Idx i = 0; i < 30; ++i) std::cout << result(i) << '\n';
```
