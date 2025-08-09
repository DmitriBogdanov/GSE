cmake_minimum_required(VERSION 3.21.0...3.22.1)

cmake_minimum_required(VERSION 3.9.1)

project(
    GSE
    VERSION 1.0.2
    DESCRIPTION "Generic ODE, SDE and algebraic solvers for Eigen."
    HOMEPAGE_URL "https://github.com/DmitriBogdanov/GSE"
)

# Declare the library
add_library               (GSE INTERFACE                                )
target_include_directories(GSE INTERFACE "${PROJECT_SOURCE_DIR}/include")
target_compile_features   (GSE INTERFACE cxx_std_17                     )

# Build benchmarks / examples / tests if project is top-level
if("${CMAKE_SOURCE_DIR}" STREQUAL "${PROJECT_SOURCE_DIR}")

    # Benchmarks
    add_subdirectory("benchmarks")
    
    # Tests & examples
    include(CTest)
    enable_testing() # should be called before adding directories with tests
    
    add_subdirectory("tests")
    add_subdirectory("examples")
    
endif()
