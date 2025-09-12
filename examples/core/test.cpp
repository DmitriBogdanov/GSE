#include "GSE/core/types.hpp"
#include "GSE/core/init.hpp"
#include "GSE/linear/solver.hpp"

int main() {
    static_assert(gse::linear::method_function<gse::linear::methods::PartialPivotLU<float, 3>, float, 3>);
    
    auto x = gse::init::zero<float, 4>(15);
}