# Add dependencies
add_library               (UTL INTERFACE             )
target_include_directories(UTL INTERFACE "firstparty")
target_compile_features   (UTL INTERFACE cxx_std_17  )

# Build examples
function(gse_add_example file_name)
    string(REPLACE "/" "-" target_name "${file_name}") # CMake doesn't like target names with slashes
    string(PREPEND target_name "example-")             # so we don't intersect names with benchmarks & tests
    
    add_executable(${target_name} ${file_name}.cpp)
    
    target_compile_features(${target_name} PRIVATE ${GSE_COMPILE_FEATURES} )
    target_compile_options (${target_name} PRIVATE ${GSE_TEST_FLAGS}       )
    target_link_options    (${target_name} PRIVATE ${GSE_TEST_LINKER_FLAGS})
    target_link_libraries  (${target_name} PRIVATE GSE UTL                 )
    
    add_test(
        NAME ${target_name}
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${target_name} --no-intro --no-path-filenames --force-colors
    ) # all examples double as tests
    
endfunction()

gse_add_example("module_ode/harmonic_oscillator")
