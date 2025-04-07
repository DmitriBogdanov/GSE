#include "GSE/GSE.hpp"

#include "GSE/detail/core.hpp"
#include "GSE/detail/ode.hpp"
#include "firstparty/UTL.hpp" // JSON, progressbars, etc.
#include <filesystem>
#include <limits>
#include <vector>

using namespace utl;

// ===================
// --- ODE Problem ---
// ===================

namespace problem {
// Analytical solution
constexpr auto analytical_solution = [](gse::Scalar t) -> gse::Vector<2> {
    return {std::exp(std::sin(math::sqr(t))), std::exp(std::cos(math::sqr(t)))};
};

constexpr gse::Uint nvar = 1;

// Definition
constexpr auto f = [](gse::Scalar t, const gse::Vector<2>& y0) -> gse::Vector<2> {
    return {2. * t * y0[0] * std::log(std::max(y0[1], 1e-3)), -2. * t * y0[1] * std::log(std::max(y0[0], 1e-3))};
};

constexpr gse::Scalar t0 = nvar * 0.1;
constexpr gse::Scalar t1 = t0 + 4.;

const gse::Vector<2> y0 = analytical_solution(t0);
} // namespace problem

// =========================================
// --- Integration with table formatting ---
// =========================================

// Note 1: To use dynamically sized vectors just switch 'gse::Vector<2>' to 'gse::Vector<>'
// Note 2: Don't forget that dynamic Eigen vectors don't have an initializer-list constructor,
//         unlike their static-sized equivalents

template <class Integrator>
void solve_with_integrator(Integrator&& integrator, const std::string& path) {

    // Save initial integrator params
    const gse::Scalar tol     = integrator.tolerance;
    const gse::Scalar tau_0   = integrator.tau;
    const gse::Scalar fact    = integrator.fact;
    const gse::Scalar factmin = integrator.factmin;
    const gse::Scalar factmax = integrator.factmax;

    // Callback
    json::Node      json;
    time::Stopwatch watch;
    gse::Scalar     err_global = 0;

    const auto callback = [&](gse::Scalar t, const gse::Vector<2>& y0, const auto& integrator) {
        // Record error in max-norm
        const gse::Scalar err_local = (y0 - problem::analytical_solution(t)).cwiseAbs().maxCoeff();
        err_global                  = std::max(err_global, err_local);

        // Save solution to JSON
        json::Node step_info;

        step_info["t"]            = t;
        step_info["tau"]          = integrator.tau;
        step_info["y"]            = gse::to_std(y0);
        step_info["y_analytical"] = gse::to_std(problem::analytical_solution(t));
        step_info["err_local"]    = err_local;
        step_info["err_global"]   = err_global;
        step_info["steps_taken"]   = integrator.steps_taken;
        step_info["steps_discarded"]   = integrator.steps_discarded;

        json["solution"].push_back(std::move(step_info));
    };

    constexpr gse::Scalar callback_frequency = 0; // callback every time layer

    // Solve
    gse::ode::solve(problem::f, problem::y0, problem::t0, problem::t1, callback, callback_frequency, integrator);

    json.to_file(path);

    // Print table row for tasks (1) & (2)
    const gse::Uint steps           = integrator.steps_taken + integrator.steps_discarded;
    const gse::Uint steps_discarded = integrator.steps_discarded;
    const auto      time            = watch.elapsed_ms().count();

    table::cell(tol, tau_0, fact, factmin, factmax, steps, steps_discarded, time, err_global);
}

template <class Integrator>
Integrator create_integrator() {
    Integrator integrator;
    integrator.tolerance = 1e-4;
    integrator.tau_min   = 0; // eliminate any effect of hard-clamping step size
    integrator.tau_max   = std::numeric_limits<gse::Scalar>::max();
    return integrator;
}

int main() {
    // --- Task (1) ---
    // ----------------

    // Task: Fixed tolerance DOPRI45, play around with tau_0 / fact / factmin / factmax and record results in a table
    
    log::println();
    log::println("--- Task (1) ---");
    log::println("----------------");
    log::println();
    
    // Wrapp integrator creation in lambda to init with a different set of default params
    const auto create_dopri45 = create_integrator<gse::ode::integrators::DOPRI45<2>>;
    const auto create_rk4re = create_integrator<gse::ode::integrators::RK4RE<2>>;

    // Export path for the result files
    std::size_t counter            = 0;
    const auto  create_result_path = [&] {
        return "temp/task_1/" + stre::pad_with_leading_zeroes(++counter, 4) + ".json";
    };

    // Table setup
    const auto table_heading = []{
        table::create({18, 18, 6, 9, 9, 12, 15, 10, 17});
        table::set_formats({table::SCIENTIFIC(1), table::SCIENTIFIC(1), table::FIXED(1), table::FIXED(1), table::FIXED(1),
                            table::DEFAULT(), table::DEFAULT(), table::FIXED(2), table::SCIENTIFIC(2)});
        table::set_latex_mode(false); // <- change this to export tables straight in LaTeX format
        table::cell("$tol$", "$\\tau_0$", "$fact$", "$factmin$", "$factmax$", "Steps", "Steps discarded", "Time (ms)", "Global Error");
        table::hline();
    };
    
    table_heading();

    // Vary parameters 1-by-1, varying the whole matrix would give us M^4 combinations which is too much
    std::vector<gse::Scalar> tau_0_vals   = {1e-2, 1e-4, 1e-6, 1e-8};
    std::vector<gse::Scalar> fact_vals    = {0.5, 0.7, 0.8, 0.9};
    std::vector<gse::Scalar> factmin_vals = {0.2, 0.5, 0.7, 0.9};
    std::vector<gse::Scalar> factmax_vals = {1.2, 1.5, 3.0, 5.0};

    for (const auto& tau_0 : tau_0_vals) {
        auto integrator = create_dopri45();
        integrator.tau  = tau_0;
        solve_with_integrator(integrator, create_result_path());
    }

    for (const auto& fact : fact_vals) {
        auto integrator = create_dopri45();
        integrator.fact = fact;
        solve_with_integrator(integrator, create_result_path());
    }

    for (const auto& factmin : factmin_vals) {
        auto integrator    = create_dopri45();
        integrator.factmin = factmin;
        solve_with_integrator(integrator, create_result_path());
    }

    for (const auto& factmax : factmax_vals) {
        auto integrator    = create_dopri45();
        integrator.factmax = factmax;
        solve_with_integrator(integrator, create_result_path());
    }

    // --- Task (2) ---
    // ----------------

    // Task: Fixed set of params, compare DOPRI45 and RK4RE, record results in a table.
    //       Plot how global error / local error / tau / solution change in time.
    
    log::println();
    log::println("--- Task (2) ---");
    log::println("----------------");
    log::println();

    std::size_t counter_2            = 0;
    const auto  create_result_path_2 = [&] {
        return "temp/task_2/" + stre::pad_with_leading_zeroes(++counter_2, 4) + ".json";
    };

    // Table setup
    table_heading();

    // Vary tolerance
    std::vector<gse::Scalar> tol_vals = {1e-3, 1e-4, 1e-6};

    for (const auto& tol : tol_vals) {
        auto integrator      = create_dopri45();
        integrator.tolerance = tol;
        solve_with_integrator(integrator, create_result_path_2());
    }
    
    table::hline();
    
    for (const auto& tol : tol_vals) {
        auto integrator      = create_rk4re();
        integrator.tolerance = tol;
        solve_with_integrator(integrator, create_result_path_2());
    }
}