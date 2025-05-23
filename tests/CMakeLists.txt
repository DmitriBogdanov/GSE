include(CTest)
enable_testing()

# Macro for defining a test with proper doctest args
macro(add_gse_test filename)
    add_executable(${filename} ${filename}.cpp)
    
    target_compile_features(${filename} PRIVATE cxx_std_17)
    
    target_compile_options(${filename} PRIVATE
        -O0
        -g3
        -fno-sanitize-recover=all
        -fsanitize=undefined,address,leak
        -Wall -Wextra -Wpedantic -Werror
    )
    # '-O0  -fno-omit-frame-pointer'           => no optimization makes compilation a bit faster
    # '-g3'                                    => leave as much debug info as possible, use '-ggdb3'
    #                                             for GDB-specific format that has even more info
    # '-fno-sanitize-recover=all'              => sanitizers should stop execution (which fails the test)
    #                                             when  triggered, default behavior is to continue
    # '-fsanitize=undefined,address,leak'      => enable sanitizers
    # '-Wall -Wextra -Wpedantic -Werror'       => more warnings, treat warnings as errors
    # see GCC docs: https://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html
    #               https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html
    
    target_link_options(${filename} PRIVATE -fsanitize=undefined,address,leak)
    # sanitizers are libraries that should be linked, otherwise we would get "undefined reference to function" errors
    
    add_test(
        NAME ${filename}
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${filename} --no-intro --no-path-filenames --force-colors
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        # we want relative path to start at the top-level project directory '/',
        # by default CTest sets working directory to ${CMAKE_CURRENT_BINARY_DIR} which is '/build/tests/'
    )
    
    # Note:
    # '--force-colors' makes doctest show colored output in the terminal, but makes up CTest logs save ANSI color
    # codes. Without this flag doctest suppresses color because it detects CTest writing output to the file.
    
endmacro()

# Tests
add_gse_test(test_derivative)
add_gse_test(test_gradient)
add_gse_test(test_jacobian)
add_gse_test(test_linear)
add_gse_test(test_nonlinear)