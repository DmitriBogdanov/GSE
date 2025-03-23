cmake_minimum_required(VERSION 3.9.1)

project(project_name)
project(
    XXXXXX
    VERSION 2.0.0
    DESCRIPTION "DESCRIPTION"
    HOMEPAGE_URL "https://github.com/DmitriBogdanov/XXXXXX"
)

# Source
include_directories(source)
add_subdirectory(source)

# Tests
add_subdirectory(tests)
