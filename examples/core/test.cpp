#include "GSE/core/types.hpp"
#include "GSE/core/initialize.hpp"
#include "GSE/linear/solver.hpp"

#include <iostream>

int main() {
    auto x = gse::init::identity<float, 5, 4>();
    
    std::cout << x;
}