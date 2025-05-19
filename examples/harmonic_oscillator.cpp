#include "GSE/core.hpp"
#include "GSE/jacobian.hpp"
#include "GSE/linear.hpp"
#include "GSE/nonlinear.hpp"
#include "GSE/ode.hpp"

#include "firstparty/UTL.hpp" // JSON, progressbars, etc.

// =====================
// --- Usage Example ---
// =====================

// Note 1: To use dynamically sized vectors just switch 'gse::Vector<2>' to 'gse::Vector<>'
// Note 2: Don't forget that dynamic Eigen vectors don't have an initializer-list constructor,
//         unlike their static-sized equivalents

using Scalar = double;
using Vector = gse::Vector<Scalar, 2>;
using Matrix = gse::Matrix<Scalar, 2, 2>;

void neat_example() {
    using namespace gse;
    using Scalar  = double;
    using Vector2 = gse::Vector<Scalar, 2>;

    // Cauchy's problem for harmonic oscillator equation
    // { x' = v
    // { v' = -k/m x
    // { x(0) = 1, v(0) = 0
    const auto    f  = [](Scalar, const Vector2& u) -> Vector2 { return {u[1], -std::sqrt(2) * u[0]}; };
    const Vector2 x0 = {0, 1};
    const Scalar  t0 = 0;
    const Scalar  t1 = 10;

    // Construct a custom ODE integration method, let's say we want to use symplectic Euler's 
    // method while solving its implicit systems with Newton's method, which should use
    // central differences to compute jacobian and LU-decomposition to solve linear systems
    using ode_integrator = ode::method::SymplecticEuler<
        Scalar,
        nonlinear::method::Newton<
            jacobian::method::CentralDifference,
            linear::method::FullPivotLU
        >
    >;

    // Solve ODE
    ode::solve(f, x0, t0, t1, ode_integrator{});
}

int main() {
    // ODE Problem (harmonic oscillator equation)
    const auto f = [](Scalar, const Vector& u) -> Vector { return {u[1], -std::sqrt(2) * u[0]}; };

    const Vector y0 = {0, 1};
    const Scalar t0 = 0;
    const Scalar t1 = 10;

    // Callback to export step info into JSON
    utl::json::Node json;

    const auto callback = [&](Scalar t, const Vector& y0, const auto& integrator) {
        utl::json::Node step_info;

        step_info["t"]         = t;
        step_info["x"]         = gse::to_std(y0);
        step_info["time_step"] = integrator.time_step;

        json["solution"].push_back(std::move(step_info));
    };

    // (optional) Select integrator
    using integrator_type = gse::ode::method::SymplecticEuler<
        Scalar,
        gse::nonlinear::method::Newton<gse::jacobian::method::CentralDifference, gse::linear::method::FullPivotLU>>;

    integrator_type method;
    method.time_step = 1e-3;

    // Solve for 't' in [0, 10], export results at 100 time layers
    gse::ode::solve(f, y0, t0, t1, callback, method);

    json.to_file("temp/harmonic_oscillator.json");
}