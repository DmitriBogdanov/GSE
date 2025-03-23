add_executable(run main.cpp)

target_compile_features(run PRIVATE cxx_std_17)
target_compile_options(run PRIVATE -O2 -Wall -Wextra -Wpedantic -Werror)



# _______________________ EXAMPLES _______________________

# # Macro for defining regular executable with optimization
# macro(add_regular_executable filename)
#     add_executable(${filename} ${filename}.cpp)
#
#     target_compile_features(${filename} PRIVATE cxx_std_17)
#
#     target_compile_options(${filename} PRIVATE
#         -O2
#         -Wall -Wextra -Wpedantic -Werror
#     )
#     # '-O2'                                    => reasonable level of optimization
#     # '-Wall -Wextra -Wpedantic -Werror'       => more warnings, treat warnings as errors
# endmacro()
#
# # Macro for defining test executable with debugging & guardrails
# macro(add_test_executable filename)
#     add_executable(${filename} ${filename}.cpp)
#
#     target_compile_features(${filename} PRIVATE cxx_std_17)
#
#     target_compile_options(${filename} PRIVATE
#         -O0
#         -g3 
#         -fno-omit-frame-pointer
#         -fno-sanitize-recover=all
#         -fsanitize=undefined,address,leak
#         -Wall -Wextra -Wpedantic -Werror
#     )
#     # '-O0  -fno-omit-frame-pointer'           => no optimization makes compilation a bit faster
#     # '-g3'                                    => leave as much debug info as possible, use '-ggdb3'
#     #                                             for GDB-specific format that has even more info
#     # '-fno-omit-frame-pointer'                => doesn't matter with '-O0', prohibits optimization
#     #                                             than makes stack traces harder to analyze
#     # '-fno-sanitize-recover=all'              => sanitizers should stop execution (which fails the test)
#     #                                             when  triggered, default behaviour is to continue
#     # '-fsanitize=undefined,address,leak'      => enable sanitizers
#     # '-Wall -Wextra -Wpedantic -Werror'       => more warnings, treat warnings as errors
#     # see GCC docs: https://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html
#     #               https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html
# endmacro()
#
# add_regular_executable(main.cpp)
