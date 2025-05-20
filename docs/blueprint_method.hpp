// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DmitriBogdanov/GSE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source repo:   https://github.com/DmitriBogdanov/GSE
//
// This project is licensed under the MIT License
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

// _______________________ INCLUDES _______________________

#include "../../core/traits.hpp"
#include "../../core/types.hpp"

#include "./base.hpp"

// ____________________ DEVELOPER DOCS ____________________

// <method type>. <method name>.

// ____________________ IMPLEMENTATION ____________________

namespace gse::impl::ode::method {

template <class T, /* (!optional) SomeMethod = some::defaults::some_method<T>, ... */>
struct NewMethod : base::Common<T> {
    
    // (!optional)
    // SomeMethod some_method;
    // ...
    
    template <Extent N, class Func, require_some_function<T, N, Func> = true>
    /* return_type */ operator()(/* args... */) {
        
    }
};

} // namespace gse::impl::ode::method
