# Guide on callbacks with a control flow

## Summary

**Problem:** User might want to break from iterative method based on some condition

**Formalization:** User should be able to emulate `break` from a callback passed into a function that performs looping.

**Solution:** Optionally return `ControlFlow` enum from a callback and use it to deduce further action.

**Note:** This is similar to `ControlFlow` stuff from `Rust` programming language. Also see [this blog](https://vittorioromeo.com/index/blog/controlflow.html) by Vittorio Romeo.

## Explanation

Let's say we pass a callback to some iterative method:

```cpp
template <class Callback>
void iterative_solve(Callback&& callback) {
    for (double t = 0; t < 100; t += 1e-1) {
        // ...
        callback();
    }
}
```

If we want to stop iteration early (for example, if we detect that solution diverges) there is nothing we can currently do from a callback.

What we can do is define a `ControlFlow` enum and use its return to decide further action:

```cpp
auto callback = []{
    // ...
    return some_condition() ? ControlFlow::BREAK : ControlFlow::CONTINUE:
}

template <class Callback>
void iterative_solve(Callback&& callback) {
    for (double t = 0; t < 100; t += 1e-1) {
        // ...
        if (callback() == ControlFlow::BREAK) break;
    }
}
```

The only thing left to do is add some `constexpr` logic to make it so callbacks don't always have to return `ControlFlow`.

```cpp
template <class Callback>
void iterative_solve(Callback&& callback) {
    for (double t = 0; t < 100; t += 1e-1) {
        // ...
        if constexpr (std::is_void_v<decltype(callback())>) callback();   
        else if (callback() == ControlFlow::BREAK) break;
    }
}
```

This boilerplate can be abstracted behind a macro.

For common use cases we can also provide some utils to reduce control flow boilerplate, for example.

```cpp
auto callback = []{
    // ...
    return break_if_not_finite(some_value):
}
```

