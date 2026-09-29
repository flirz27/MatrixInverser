cmake_minimum_required(VERSION 3.29)
project(MatrixInverter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(matrix_core STATIC
        src/matrix.cpp
)

target_include_directories(matrix_core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(matrix_core PRIVATE -O2 -msse -msse2 -msse3)

add_executable(MatrixInverter src/main.cpp)
target_link_libraries(MatrixInverter PRIVATE matrix_core)

enable_testing()
find_package(GTest REQUIRED)

add_executable(Tests
        tests/tests.cpp
)
target_link_libraries(Tests PRIVATE matrix_core GTest::gtest_main pthread)

include(GoogleTest)
gtest_discover_tests(Tests)