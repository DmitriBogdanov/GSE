// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include <type_traits> // is_void_v<>

// ____________________ DEVELOPER DOCS ____________________

// See 'docs/dev_callback_control_flow.md'.

// ____________________ IMPLEMENTATION ____________________

namespace gse {

// ====================
// --- Control flow ---
// ====================

// [ LSP documentation ]
//
// Enumeration used for exposing loop control flow to the callbacks.
//
// Usually used divergence checks and other types of conditional iteration escapes.
//
enum class ControlFlow { BREAK, CONTINUE };

} // namespace gse

// Macro to generate 'if constexpr' boilerplate for callbacks with potentially
// present control flow return, perhaps this could be implemented as a function,
// but that requires some tricky logic to ensure non-controlled case doesn't
// create a runtime branch, while with macro this is trivial.
//
// Similar idea can be found in ThinkCell library where they also declare it as a macro:
// https://github.com/think-cell/think-cell-library/blob/main/tc/algorithm/break_or_continue.h
//
#define GSE_IMPL_INVOKE_WITH_CONTROL_FLOW(func_, break_)                                                               \
    if constexpr (std::is_void_v<decltype(func_)>) {                                                                   \
        func_;                                                                                                         \
    } else if (func_ == gse::ControlFlow::BREAK) {                                                                     \
        break_;                                                                                                        \
    }                                                                                                                  \
                                                                                                                       \
    static_assert(true)