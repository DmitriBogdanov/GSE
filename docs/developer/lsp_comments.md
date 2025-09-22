# Writing comments that show up in LSP

## Motivation

If a function / class / typedef has a comment directly above it most LPS will show that comment when hovering over the identifier name.

**This is rather convenient for the user** as they can get some basic documentation at a glance, without having to switch tabs to a full doc.

## Complications

### Different LSP engines

There are 2 main LSP engines: `clangd` and Visual Studio `IntelliSense`.

Those engines have slightly different rules when it comes to deciding which comments to show and how to interact with `using` directives.

Let's consider following example:

```cpp
namespace gse::impl {
    // [comment 1]
    void some_function() {}
}

namespace gse {
    // [comment 2]
    using impl::some_function;
}
```

When hovering over `some_function`, `clangd` will pick up the `[comment 2]` from the `using` directive. `IntelliSense` on the other hand will show `[comment 1]` from the original definition.

If we were only concerned with supporting `clangd` it would be quite convenient to isolate publicly exposed API into its own place with all of the LSP comments. However, since we want to support both commonly used engines we have to write things in a way that works similarly for both. This rules out the `using` directives, so we structure the code to always expose things directly:

```cpp
namespace gse {
    // [comment]
    void some_function() {}
}
```

### Line breaking rules

LSPs have somewhat unintuitive rules when it comes to placing line breaks in a visually displayed text. `clangd`, for example, uses punctuation & empty lines to decide linebreaks:

```cpp
// [line 1]
// [line 1]
// [line 1]
void some_function() {}

// [line 1]
//
// [line 2]
void some_function() {}

// [line 1]
//
// [line 2]
//
void some_function() {}

// [line 1],
// [line 2],
// [line 3].
void some_function() {}
```

`IntelliSense` on the other hand preserves line breaks & spacing in a sensible way, so we use what works with `clangd` and `IntelliSense` will be always compatible.

### Comments above macros

`clangd` doesn't support LSP comments for macros nothing we can do about it. `IntelliSense` works fine.

## Why not use doxygen comments

Doxygen comments require special support from the LSP in order to look readable. `IntelliSense` provides such support, `clangd` currently doesn't. Using the rules below the comments end up looking good regardless of LSP.

## Comment guidelines

All public API has to provide LSP comments with the following format:

```cpp
// [ LSP documentation ]
//
// <content>
//
```

This somewhat increases code verbosity, but results in a rather nice LSP result. Some codebases use Doxygen comments for the same purpose, however their format is not as readable.
