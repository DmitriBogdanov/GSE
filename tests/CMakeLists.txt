# Add dependencies
add_library               (doctest INTERFACE             )
target_include_directories(doctest INTERFACE "thirdparty")
target_compile_features   (doctest INTERFACE cxx_std_17  )

# Build tests
function(gse_add_test file_name)
    string(REPLACE "/" "-" target_name "${file_name}") # CMake doesn't like target names with slashes
    string(PREPEND target_name "test-")                # so we don't intersect names with benchmarks & examples
    
    add_executable(${target_name} ${file_name}.cpp)
    
    target_compile_features(${target_name} PRIVATE ${GSE_COMPILE_FEATURES} )
    target_compile_options (${target_name} PRIVATE ${GSE_TEST_FLAGS}       )
    target_link_options    (${target_name} PRIVATE ${GSE_TEST_LINKER_FLAGS})
    target_link_libraries  (${target_name} PRIVATE GSE doctest             )
    
    add_test(
        NAME ${target_name}
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${target_name} --no-intro --no-path-filenames --force-colors
    )
    
endfunction()

# Note: '--force-colors' makes doctest show colored output in the terminal, but makes up CTest logs save ANSI
#       color codes. Without this flag doctest suppresses color because it detects CTest writing output to the file.

gse_add_test("module_derivative")
gse_add_test("module_gradient")
gse_add_test("module_jacobian")
gse_add_test("module_linear")
gse_add_test("module_nonlinear")
gse_add_test("module_ode")
