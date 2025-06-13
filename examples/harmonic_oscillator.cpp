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

template <class T1, class T2 = float>
struct TestStruct {};

int main() {
    // ODE Problem (harmonic oscillator equation)
    const auto f = [](Scalar, const Vector& u) -> Vector { return {u[1], -std::sqrt(2) * u[0]}; };

    const Vector x0 = {0, 1};
    const Scalar t0 = 0;
    const Scalar t1 = 10;

    // Callback to export step info into JSON
    utl::json::Node json;

    const auto callback = [&](Scalar t, const Vector& y0, const auto& integrator) {
        utl::json::Node step_info;
        
        if (t > 5)  {
            utl::log::println("Solution diverges at t = ", t);
            return gse::ControlFlow::BREAK;
        }

        step_info["t"]         = t;
        step_info["x"]         = gse::to_std(y0);
        step_info["time_step"] = integrator.time_step;

        json["solution"].push_back(std::move(step_info));
        
        return gse::ControlFlow::CONTINUE;
    };

    // (optional) Select integrator
    using custom_ode_method = gse::ode::method::SymplecticEuler<
        double, gse::nonlinear::method::Newton<double, gse::jacobian::method::CentralDifference<double>,
                                               gse::linear::method::HouseholderQR>>;

    custom_ode_method method;
    method.time_step                                  = 1e-3; // ODE method time step
    method.nonlinear_method.precision                 = 1e-6; // internal Newton's method precision
    method.nonlinear_method.jacobian_method.diff_step = 1e-5; // internal jacobian numerical diff. step
    // ...

    // Solve for 't' in [0, 10], export results at 100 time layers
    utl::time::Stopwatch watch;
    gse::ode::solve(f, x0, t0, t1, callback, method);
    utl::log::println(watch.elapsed_string());

    json.to_file("temp/harmonic_oscillator.json");
}