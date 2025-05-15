#include <chrono>
#include <complex>
#include <string>
#include <utility>
#include <vector>

#include <execinfo.h>

#include "firstparty/UTL.hpp"

#define BACKWARD_HAS_DW 1
//#define BACKWARD_HAS_BFD 1
//#define BACKWARD_HAS_DWARF 1
#include "thirdparty/backward.hpp"

#define ANKERL_NANOBENCH_IMPLEMENT
#include "thirdparty/nanobench.h"

// Note:
// This is a common include for all benchmarks,
// it exists purely to reduce boilerplate and shouldn't be included anywhere else.

#define REPEAT(repeats_) for (int count_ = 0; count_ < repeats_; ++count_)
#define DO_NOT_OPTIMIZE_AWAY ankerl::nanobench::doNotOptimizeAway

inline ankerl::nanobench::Bench bench;

template <class Func>
void benchmark(const char* name, Func lambda) {
    bench.run(name, lambda);
}

template <class Func>
void benchmark(const std::string &name, Func lambda) {
    bench.run(name.c_str(), lambda);
}

using namespace utl;
using namespace std::chrono_literals;
using namespace std::string_literals;

namespace backward {

backward::SignalHandling sh;

}