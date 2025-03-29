#include "GSE/GSE.hpp"

#include "GSE/detail/ode.hpp"
#include "firstparty/UTL.hpp" // JSON, progressbars, etc.

// =====================
// --- Usage Example ---
// =====================

// Note 1: To use dynamically sized vectors just switch 'gse::Vector<2>' to 'gse::Vector<>'
// Note 2: Don't forget that dynamic Eigen vectors don't have an initializer-list constructor,
//         unlike their static-sized equivalents

int main() {
    // ODE Problem (harmonic oscillator equation)
    constexpr auto f = [](gse::Scalar, const gse::Vector<2>& y0) -> gse::Vector<2> {
        constexpr gse::Scalar w = 2.;
        return {y0[1], -std::sqrt(w) * y0[0]};
    };

    const gse::Vector<2>  y0 = {0, 1};
    constexpr gse::Scalar t0 = 0;
    constexpr gse::Scalar t1 = 1e+3;

    // Callback (show progressbar and write result to JSON)
    using namespace utl;

    json::Node              json;
    progressbar::Percentage bar;

    const auto callback = [&](gse::Scalar t, const gse::Vector<2>& y0, const auto& integrator) {
        json::Node step_info;
        step_info["t"]   = t;
        step_info["y"]   = gse::to_std(y0);
        step_info["tau"] = integrator.tau;

        json["solution"].push_back(std::move(step_info));

        bar.set_progress((t - t0) / (t1 - t0));
    };

    constexpr gse::Scalar callback_frequency = (t1 - t0) / 100; // save 100 time layers in total
    // set '0' to save every time layer
    
    // Integrator (optional)
    gse::ode::integrators::DOPRI45<2> integrator;
    integrator.tau       = 3e-1;
    integrator.tolerance = 1e-7;
    integrator.tau_max   = 5e-1;

    // Divergence checking (optional)
    constexpr bool verify = true;

    // Solve
    gse::ode::solve(f, y0, t0, t1, callback, callback_frequency, integrator, verify);

    json.to_file("temp/gse_solution.json");
    bar.finish();
}