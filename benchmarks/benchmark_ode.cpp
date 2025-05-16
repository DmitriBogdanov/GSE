// __________ BENCHMARK FRAMEWORK & LIBRARY  __________

#include "GSE/ode.hpp"

#include "benchmark.hpp"
#include <dwarf.h>
#include <functional>

// _____________ BENCHMARK IMPLEMENTATION _____________

// --- Bench a single RHS / integrator row ---
// -------------------------------------------

constexpr gse::Extent N   = 6; // smaller 'N' should put more stress on allocator
constexpr gse::Scalar c   = N;
constexpr gse::Scalar T   = 5;
constexpr gse::Scalar tau = 1e-5;

namespace integrators = gse::ode::integrators;

template <class Func, class Integrator>
void bench_sample(Func&& f, Integrator&& integrator, const std::string f_name, const std::string integrator_name) {
    using Vec = decltype(f(0, gse::Vector<>()));

    const Vec y0 = gse::Vector<>::Random(N);

    gse::Scalar sum{};
    const auto  callback = [&](gse::Scalar t, const auto& y0, const auto& integrator) {
        sum += t + y0[0] + integrator.tau;
    };

    const gse::Scalar callback_frequency = T / 2;

    integrator.tau = tau;

    const std::string name = stre::pad_right(f_name, 25) + "   |   " + stre::pad_right(integrator_name, 25);

    benchmark(name, [&]() {
        gse::ode::solve(f, y0, 0., T, callback, callback_frequency, integrator, false);
        DO_NOT_OPTIMIZE_AWAY(sum);
    });
}

#define BENCH_SAMPLE(f_, integrator_) bench_sample(f_, integrator_, #f_, #integrator_)

// --- Different kinds of RHS ---
// ------------------------------

// f_compile_lambda
// f_compile_struct
// f_compile_function
// f_dynamic_lambda
// f_dynamic_struct
// f_dynamic_function

const auto f_compile_lambda = [](gse::Scalar, const gse::Vector<N>& y0) -> gse::Vector<N> {
    gse::Vector<N> res;
    for (gse::Idx i = 0; i < res.size(); ++i) res[i] = y0[i] * i / c;
    return res;
};

const auto f_dynamic_lambda = [](gse::Scalar, const gse::Vector<>& y0) -> gse::Vector<> {
    gse::Vector<> res = gse::Vector<>::Zero(N);
    for (gse::Idx i = 0; i < res.size(); ++i) res[i] = y0[i] * i / c;
    return res;
};

const std::function<gse::Vector<N>(gse::Scalar, gse::Vector<N>)> f_compile_stdfun = f_compile_lambda;

const std::function<gse::Vector<>(gse::Scalar, gse::Vector<>)> f_dynamic_stdfun = f_dynamic_lambda;

struct {
    gse::Vector<N> res{};

    const gse::Vector<N>& operator()(gse::Scalar, const gse::Vector<N>& y0) {
        for (gse::Idx i = 0; i < res.size(); ++i) res[i] = y0[i] * i / c;
        return res;
    }
} f_compile_struct;

struct {
    gse::Vector<> res = gse::Vector<>::Zero(N);

    const gse::Vector<>& operator()(gse::Scalar, const gse::Vector<>& y0) {
        for (gse::Idx i = 0; i < res.size(); ++i) res[i] = y0[i] * i / c;
        return res;
    }
} f_dynamic_struct;

// --- Different kinds of Integrators ---
// --------------------------------------

template <gse::Extent N = gse::dynamic_size>
struct RK4_reuse_k {
    // - Params -
    gse::Scalar tau = gse::ode::defaults::tau;

    // - Integrator -
    template <class Func>
    void operator()(Func&& f, gse::Scalar& t, gse::Vector<N>& y0) {
        k1 = f(t, y0);
        k2 = f(t + 0.5 * tau, y0 + 0.5 * tau * k1);
        k3 = f(t + 0.5 * tau, y0 + 0.5 * tau * k2);
        k4 = f(t + tau, y0 + tau * k3);

        y0 += tau / 6. * (k1 + 2. * k2 + 2. * k3 + k4);

        t += tau;
    }
private:
    gse::Vector<N> k1, k2, k3, k4;
};

template <gse::Extent N = gse::dynamic_size>
struct RK4_naive {
    // - Params -
    gse::Scalar tau = gse::ode::defaults::tau;

    // - Integrator -
    template <class Func>
    void operator()(Func&& f, gse::Scalar& t, gse::Vector<N>& y0) {
        const gse::Vector<N> k1 = f(t, y0);
        const gse::Vector<N> k2 = f(t + 0.5 * tau, y0 + 0.5 * tau * k1);
        const gse::Vector<N> k3 = f(t + 0.5 * tau, y0 + 0.5 * tau * k2);
        const gse::Vector<N> k4 = f(t + tau, y0 + tau * k3);

        y0 += tau / 6. * (k1 + 2. * k2 + 2. * k3 + k4);

        t += tau;
    }
};

// --- Run benchmarks ---
// ----------------------

void benchmark_ode_solvers() {

    bench.timeUnit(1ms, "ms").warmup(10).minEpochTime(50ms).relative(true);
    
    bench.title("Euler");
    BENCH_SAMPLE(f_compile_struct, integrators::Euler<N>{});
    BENCH_SAMPLE(f_compile_lambda, integrators::Euler<N>{});
    BENCH_SAMPLE(f_compile_stdfun, integrators::Euler<N>{});
    BENCH_SAMPLE(f_dynamic_struct, integrators::Euler<>{});
    BENCH_SAMPLE(f_dynamic_lambda, integrators::Euler<>{});
    BENCH_SAMPLE(f_dynamic_stdfun, integrators::Euler<>{});
    
    bench.title("RK4");
    BENCH_SAMPLE(f_compile_struct, integrators::RK4<N>{});
    BENCH_SAMPLE(f_compile_lambda, integrators::RK4<N>{});
    BENCH_SAMPLE(f_compile_stdfun, integrators::RK4<N>{});
    BENCH_SAMPLE(f_dynamic_struct, integrators::RK4<>{});
    BENCH_SAMPLE(f_dynamic_lambda, integrators::RK4<>{});
    BENCH_SAMPLE(f_dynamic_stdfun, integrators::RK4<>{});
    
    BENCH_SAMPLE(f_compile_struct, RK4_reuse_k<N>{});
    BENCH_SAMPLE(f_compile_lambda, RK4_reuse_k<N>{});
    BENCH_SAMPLE(f_compile_stdfun, RK4_reuse_k<N>{});
    BENCH_SAMPLE(f_dynamic_struct, RK4_reuse_k<>{});
    BENCH_SAMPLE(f_dynamic_lambda, RK4_reuse_k<>{});
    BENCH_SAMPLE(f_dynamic_stdfun, RK4_reuse_k<>{});
    BENCH_SAMPLE(f_compile_struct, RK4_naive<N>{});
    BENCH_SAMPLE(f_compile_lambda, RK4_naive<N>{});
    BENCH_SAMPLE(f_compile_stdfun, RK4_naive<N>{});
    BENCH_SAMPLE(f_dynamic_struct, RK4_naive<>{});
    BENCH_SAMPLE(f_dynamic_lambda, RK4_naive<>{});
    BENCH_SAMPLE(f_dynamic_stdfun, RK4_naive<>{});
    
    bench.title("AdamsRK4");
    BENCH_SAMPLE(f_compile_struct, integrators::AdamsRK4<N>{});
    BENCH_SAMPLE(f_compile_lambda, integrators::AdamsRK4<N>{});
    BENCH_SAMPLE(f_compile_stdfun, integrators::AdamsRK4<N>{});
    BENCH_SAMPLE(f_dynamic_struct, integrators::AdamsRK4<>{});
    BENCH_SAMPLE(f_dynamic_lambda, integrators::AdamsRK4<>{});
    BENCH_SAMPLE(f_dynamic_stdfun, integrators::AdamsRK4<>{});
    
    bench.title("RK4RE");
    BENCH_SAMPLE(f_compile_struct, integrators::RK4RE<N>{});
    BENCH_SAMPLE(f_compile_lambda, integrators::RK4RE<N>{});
    BENCH_SAMPLE(f_compile_stdfun, integrators::RK4RE<N>{});
    BENCH_SAMPLE(f_dynamic_struct, integrators::RK4RE<>{});
    BENCH_SAMPLE(f_dynamic_lambda, integrators::RK4RE<>{});
    BENCH_SAMPLE(f_dynamic_stdfun, integrators::RK4RE<>{});
    
    bench.title("DOPRI45");
    BENCH_SAMPLE(f_compile_struct, integrators::DOPRI45<N>{});
    BENCH_SAMPLE(f_compile_lambda, integrators::DOPRI45<N>{});
    BENCH_SAMPLE(f_compile_stdfun, integrators::DOPRI45<N>{});
    BENCH_SAMPLE(f_dynamic_struct, integrators::DOPRI45<>{});
    BENCH_SAMPLE(f_dynamic_lambda, integrators::DOPRI45<>{});
    BENCH_SAMPLE(f_dynamic_stdfun, integrators::DOPRI45<>{});
}

int main() {
    benchmark_ode_solvers();
}