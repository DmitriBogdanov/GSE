// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include <type_traits> // is_void

// ____________________ DEVELOPER DOCS ____________________

// Callback control flow enum for callback, needed to break early from iterative methods.
//
// Full explanation can be found in 'docs/dev_callback_control_flow.md'.

// ____________________ IMPLEMENTATION ____________________

namespace gse::impl {

// ==========================
// --- Core functionality ---
// ==========================

enum class ControlFlow { BREAK, CONTINUE };

// Macro to generate 'if constexpr' boilerplate for callbacks with potentially
// present control flow return, perhaps this could be implemented as a function,
// but that requires some tricky logic to ensure non-controlled case doesn't
// create a runtime branch, while with macro this is trivial
//
// Similar idea can be found in Think-Cell library where they also declare it as a macro:
// https://github.com/think-cell/think-cell-library/blob/main/tc/algorithm/break_or_continue.h
//
#define GSE_IMPL_INVOKE_WITH_CONTROL_FLOW(func_, break_)                                                               \
    if constexpr (std::is_void_v<decltype(func_)>) {                                                                   \
        func_;                                                                                                         \
    } else if (func_ == ControlFlow::BREAK) {                                                                          \
        break_;                                                                                                        \
    }                                                                                                                  \
                                                                                                                       \
    static_assert(true)

} // namespace gse::impl