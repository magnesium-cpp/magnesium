cmake_minimum_required(VERSION 3.29)

project(magnesium VERSION 0.1.0
                  DESCRIPTION "Personal utility library."
                  LANGUAGES CXX)

set(HEADER_LIST
    "include/mg/collections.hpp"
    "include/mg/functional.hpp"
    "include/mg/math.hpp"
    "include/mg/sequence.hpp"
    "include/mg/types.hpp"
    "include/mg/utility.hpp")
add_library(magnesium INTERFACE ${HEADER_LIST})
target_include_directories(magnesium INTERFACE "include")

# Actions to take only if this is the main project being built.
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    target_compile_features(magnesium INTERFACE cxx_std_20)
    set_target_properties(magnesium PROPERTIES CXX_EXTENSIONS OFF)
    add_subdirectory(tests)
endif()
