# Guide on passing template callables

## Why not use `std::function`

`std::function` is slower that using a callable "by itself", in a nutshell:

```cpp
void use_callable(std::function<void(int)> f) { f(123); )
// slow, calling f() by itself would be faster

template<class Func>
void use_callable(Func&& f) { f(123); )
// fast, calling f() by itself would likely not have any difference
```

### Slowdown reason 1

`std::function` is a type-erased callable, to perform type-erasure it needs to have additional indirection internally.

This indirection introduces inherent overhead on each call, which in many cases ends up being significant.

### Slowdown reason 2

Due to type-erasure `std::function` prevents compiler from trying to inline & optimize function that was passed as an argument.

### Slowdown reason 3

`std::funtion` might allocate when wrapping a functor with large state. Pass-by-forwarding-reference doesn't suffer from that.

## How `std` does it

`std` functions passes callables like this:

```cpp
template<class RandomIt, class Compare>
void sort(RandomIt first, RandomIt last, Compare comp); // almost good
```

This is good enough for most cases, but not perfect - it takes callable by copy which migh end up expensive if callable manages some dynamic resources.

We can improve it by using **perfect forwaring** like this:

```cpp
template<class RandomIt, class Compare>
void sort(RandomIt first, RandomIt last, Compare&& comp); // perfect
```

This avoid copying and allows the exact same semantics.

**Trivia:** `std` doesn't use perfect forwarding here due to legacy reasons. If `<algorithm>` was developed today it probably would.

We could use other types of references, but that has significant downsides:

```cpp
// Const-reference
template<class RandomIt, class Compare>
void sort(RandomIt first, RandomIt last, Compare&& comp);
// bad, can't pass callable that modifies its internal state

// l-value reference
template<class RandomIt, class Compare>
void sort(RandomIt first, RandomIt last, Compare& comp);
// meh, doesn't accept r-values, won't work if we pass lambda straight into the function
```

**TLDR: Use template functions with perfectly-forwarded callables. It can be ugly, but this is the correct way.**

## Examples

### Pass callable that will be called only once

```cpp
template<class Func>
void use_callable(Func&& f) { std::forward<Func>(f)("argument"); }
```
### Pass callable that will be called multiple times

```cpp
template<class Func>
void use_callable(Func&& f) { f("argument"); }
```

### Pass callable that has a default value

```cpp
template<class Func = DefaultFunc>
void use_callable(Func&& f = Func{}) { f("argument"); }
```

### Pass callable & restrict its signature with SFINAE

```cpp
template<
    class Func = DefaultFunc,
    _require_callable_r<Ret, Func, Arg1, Arg2> = true
    // 'f' has signature 'Ret f(Arg1, Arg2)'
    // or 'const Ret& f(const Arg1&, const Arg2&)', that will work too
>
void use_callable(Func&& f) { f("argument"); }
```

### Pass callable that has a default value & restrict its signature with SFINAE to l-value references

```cpp
template<
    class Func = DefaultFunc,
    _require_callable_r<Ret, Func, Arg1&, Arg2&> = true
    // 'f' has signature 'Ret f(Arg1&, Arg2&)'
>
void use_callable(Func&& f = Func{}) { f("argument"); }
```

### Pass callable that contains another callable in its signature

```cpp
template<
    class Func     = DefaultFunc,
	class Callback = DefaultCallback,
    _require_callable_r<Ret, Func, std::decay_t<Callback>> = true
    // 'f' has signature 'Ret f(Callback&&)', notice that 'Callback' needs to be
    // wrapped in 'std::decay_t<>' because it's perfectly-forwarded
>
void use_callable(
        Func&&     f = Func{},
        Callback&& c = Callback{}
) {
	f("argument");
}
```