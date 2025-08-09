# Add dependencies
add_library               (nanobench INTERFACE             )
target_include_directories(nanobench INTERFACE "thirdparty")
target_compile_features   (nanobench INTERFACE cxx_std_17  )

# Build benchmarks
function(gse_add_benchmark file_name)
    string(REPLACE "/" "-" target_name "${file_name}") # CMake doesn't like target names with slashes
    string(PREPEND target_name "benchmark-") # so we don't intersect names with tests & examples
    
    add_executable(${target_name} ${file_name}.cpp)
    
    target_compile_features(${target_name} PRIVATE ${GSE_COMPILE_FEATURES}      )
    target_compile_options (${target_name} PRIVATE ${GSE_BENCHMARK_FLAGS}       )
    target_link_options    (${target_name} PRIVATE ${GSE_BENCHMARK_LINKER_FLAGS})
    target_link_libraries  (${target_name} PRIVATE GSE nanobench                )
    
endfunction()

gse_add_benchmark("module_ode/dynamic_system")
