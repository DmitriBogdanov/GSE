cmake_minimum_required(VERSION 3.9.1)

project(project_name)
project(
    GSE
    VERSION 1.0.1
    DESCRIPTION "Generic ODE, SDE and algebraic solvers for Eigen."
    HOMEPAGE_URL "https://github.com/DmitriBogdanov/GSE"
)

# Headers
include_directories(include)

# Executables
add_subdirectory(tests)
add_subdirectory(benchmarks)
add_subdirectory(examples)

