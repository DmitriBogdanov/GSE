# Guide on Eigen3 quirks

## Passing matrices by value

### Summary

**Alignment issues:** Below C++17 Eigen has a whole bunch of alignment quirks, but since we target C++17 we can ignore them.

**Base class arguments:** Technically the optimal way to pass Eigen objects is by using their `MatrixBase<Derived>` and etc. This is somewhat ugly so we leave it for the future work as an optional optimization. See explanation below.

### R-value reuse below C++17

If matrices cannot be passed by value, how do we nicely write functions that can benefit from an r-value argument:

```cpp
// Pass-by-value:
// [+]: Looks clean, moves if 'x' is an r-value, allocates otherwise
// [-]: Prohibited by Eigen
template <class T, Extent N>
Vector<T, N> transform(Vector<T, N> x) {
    x[0] += 1;
    return x;
}

// Pass-by-const-ref + pass-by-rvalue-ref
// [+]: Moves if 'x' is an r-value, allocates otherwise
// [-]: Duplicates code
template <class T, Extent N>
Vector<T, N> transform(const Vector<T, N> &x) {
    Vector<T, N> res = x;
    res[0] += 1;
    return res;
}
template <class T, Extent N>
Vector<T, N> transform(Vector<T, N> &&x) {
    Vector<T, N> res = std::move(x);
    res[0] += 1;
    return res;
}

// Perfect forwarding
// [+]: No code duplication, same performance as variants above
// [-]: We no longer have explicit access to 'N' and 'T', it is possible to extract them,
//      but it requires ugly boilerplate with type traits
template <class Vec>
Vec transform(Vec&& x) {
    Vec res = std::forward<Vec>(x);
    res[0] += 1;
    return res;
}

// Using the Eigen base class (like documentation advices)
// [+]: Can take any kind of expression, potentially avoid a temporary
// [-]: Looks absolutely vile. 'N' and 'T' are extremely ugly to extract.
//      No good way to separate vectors from matrices, only assert "clutches".
template <class Derived>
Vector<meta::value_type<Derived>, meta::rows<Derived>> transform(const MatrixBase<Derived>& x) {
    ASSERT_VECTOR(MatrixBase<Derived>);
    Vector<meta::value_type<Derived>, meta::rows<Derived>> res = x;
    res[0] += 1;
    return res;
}
```

Every "correct" way is either horrible to write or inefficient compared to a much simpler pass-by-value version that would work if Eigen pass-by-value wasn't a silent landmine.

**Possible solution 1:** Wrap Eigen vectors and matrices into a custom class, that prohibits explicit copy and uses `.clone()` method instead. This will also allow us to fix some weird choices in Eigen's API. Downside: Very difficult to implement, more layers of abstraction for library users.

**Possible solution 2:** Use `Blaze` instead of Eigen. It doesn't seem to suffer from so many quirks, however the library is much less known than Eigen and doesn't provide efficient `gemm` and decompositions without linking BLAS / LAPACK, which goes against the "single include" principle.

**Possible solution 3:** Just take everything by const reference and tank the copies.

**Possible solution 4:** Write everything in terms of rvalue-references and write const reference overloads that simply do an explicit copy and wrap the other overload.

**Possible solution 5:** Try to come up with a good set of type traits and base templates that would make using Eigen base classes not horribly ugly.

```cpp
// It looks like we can nicely implement solution 5 by using 'MatrixBase' combined
// with partial a more strongly specialized template
// [+]: Preserves Eigen's expression templates, doesn't look that bad
// [-]: Doesn't utilize r-value arguments
template <class T, Extent N>
Vector<T, N> transform(const MatrixBase<Vector<T, N>>& x) {
    Vector<T, N> res = x;
    res[0] += 1;
    return res;
}
// now it seems there is no choise left other than to either tank the lack of r-value support
// or to provide and an explicit r-value overload with code duplication
template <class T, Extent N>
Vector<T, N> transform(MatrixBase<Vector<T, N>>&& x) {
    Vector<T, N> res = std::move(x);
    res[0] += 1;
    return res;
}
```

**Possible solution 6:** All this alignment nonsense doesn't actually matter if we're targeting C++17 ([see](https://eigen.tuxfamily.org/dox/group__TopicUnalignedArrayAssert.html#getrid)) and we can just leave things be as they are. This is slightly less efficient than option (5) due to not having full "integration" with Eigen expression templates, but there is a good chance it doesn't actually matter enough to be worth the complexity. Perhaps it can work as an optional overload for more optimization should we want it in the future.

## Links

- [Eigen: Passing by value](https://eigen.tuxfamily.org/dox/group__TopicPassingByValue.html)
- [Eigen: Functions taking generic types](https://eigen.tuxfamily.org/dox/TopicFunctionTakingEigenTypes.html)
- [Eigen: Aligned storage, unaligned access and disabling overalignment](https://eigen.tuxfamily.org/dox/group__TopicUnalignedArrayAssert.html#getrid)
- [Blaze: README](https://bitbucket.org/blaze-lib/blaze/src/master/)
- [Blaze: Blaze vs Eigen3 vs Armadillo vs BLAS benchmarks](https://bitbucket.org/blaze-lib/blaze/wiki/Benchmarks)
- [Medium Blaze/Eigen3/Fastor/Armadillo/XTensor benchmark](https://romanpoya.medium.com/a-look-at-the-performance-of-expression-templates-in-c-eigen-vs-blaze-vs-fastor-vs-armadillo-vs-2474ed38d982)