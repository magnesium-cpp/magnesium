cmake_minimum_required(VERSION 3.29)

include(FetchContent)
FetchContent_Declare(
    gtest
    GIT_REPOSITORY "https://github.com/google/googletest.git"
    GIT_TAG v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest)

set(SRC_LIST
    "functional_tests.cpp"
    "math_tests.cpp"
    "types_tests.cpp")

add_executable(magnesium_test ${SRC_LIST})
target_link_libraries(magnesium_test PRIVATE magnesium)
target_link_libraries(magnesium_test PRIVATE GTest::gtest_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(magnesium_test)
