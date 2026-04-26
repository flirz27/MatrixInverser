cmake_minimum_required(VERSION 3.29)
project(MatrixInverter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -msse -msse2 -msse3")

add_executable(MatrixInverter src/main.cpp
        src/matrix.cpp)

target_compile_options(MatrixInverter PRIVATE "-O2")

enable_testing()

find_package(GTest REQUIRED)

add_executable(Tests src/matrix.cpp
        tests/tests.cpp
        tests/tests.cpp)
target_link_libraries(Tests GTest::gtest_main pthread)

include(GoogleTest)
gtest_discover_tests(Tests)
