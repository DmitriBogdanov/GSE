# include(FetchContent)

# Also requires one of: libbfd (gnu binutils), libdwarf, libdw (elfutils)
# FetchContent_Declare(backward
#     GIT_REPOSITORY https://github.com/bombela/backward-cpp
#     GIT_TAG master  # or a version tag, such as v1.6
# )
# FetchContent_MakeAvailable(backward)

# Add Backward to your target (either Backward::Interface, Backward::Object, or Backward::Backward)
# target_link_libraries(mytarget PUBLIC Backward::Interface)

# Macro for defining a benchmark with proper compile options
# macro(add_benchmark filename)
#     add_executable(${filename} ${filename}.cpp)
#     target_compile_features(${filename} PRIVATE cxx_std_17)
#     target_compile_options(${filename} PRIVATE -O2 -g -ldw -Wall -Wextra -Wpedantic -fno-unroll-loops -fmax-errors=10)
#     target_link_libraries(${filename} PRIVATE Backward::Interface)
# endmacro()

# TEMP:
# add_benchmark(benchmark_ode)

# Link OpenMP if doing benchmarks with it.
# Don't forget to add '-fopenmp' to 'target_compile_options' 
# find_package(OpenMP REQUIRED)
# target_link_libraries(benchmark_parallel PRIVATE OpenMP::OpenMP_CXX)

# Profiling flags for 'perf':
# -O2 -g -ggdb -fno-omit-frame-pointer