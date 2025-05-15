#include "GSE/ode.hpp"

#include "firstparty/UTL.hpp" // JSON, progressbars, etc.

// =====================
// --- Usage Example ---
// =====================

// Note 1: To use dynamically sized vectors just switch 'gse::Vector<2>' to 'gse::Vector<>'
// Note 2: Don't forget that dynamic Eigen vectors don't have an initializer-list constructor,
//         unlike their static-sized equivalents

template <gse::Extent N = gse::dynamic_size>
struct RK2 {

    gse::Scalar    time_step = gse::ode::defaults::tau;
    gse::Vector<N> k1;
    gse::Vector<N> k2;

    template <class Func>
    void operator()(Func&& f, gse::Scalar& t, gse::Vector<N>& y0) {
        
        this->k1 = f(t, y0);
        this->k2 = f(t + this->time_step, y0 + this->time_step * k1);
        
        y0 += this->time_step * (0.5 * k1 + 0.5 * k2);
        t += this->time_step;
    }
};

int main() {
    // ODE Problem (harmonic oscillator equation)
    const auto f = [](gse::Scalar, const gse::Vector<2>& u) -> gse::Vector<2> { return {u[1], -std::sqrt(2) * u[0]}; };

    const gse::Vector<2> y0 = {0, 1};
    const gse::Scalar    t0 = 0;
    const gse::Scalar    t1 = 10;

    // Callback to export step info into JSON
    utl::json::Node json;

    const auto callback = [&](gse::Scalar t, const gse::Vector<2>& y0, const auto& integrator) {
        utl::json::Node step_info;

        step_info["t"]   = t;
        step_info["y"]   = gse::to_std(y0);
        step_info["tau"] = integrator.tau;

        json["solution"].push_back(std::move(step_info));
    };

    // (optional) Select integrator
    gse::ode::integrators::DOPRI45<2> integrator;
    integrator.tau       = 1e-3;
    integrator.tolerance = 1e-6;

    // Solve for 't' in [0, 10], export results at 100 time layers
    gse::ode::solve(f, y0, t0, t1, callback, (t1 - t0) / 100, integrator);

    json.to_file("temp/harmonic_oscillator.json");
}