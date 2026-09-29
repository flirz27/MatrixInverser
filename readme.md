MatrixInverser

A high-performance C++20 programm for SIMD-accelerated matrix inversion.


-Features

    SIMD-accelerated (SSE) element-wise addition, subtraction, and multiplication

    Matrix inversion via Neumann series approximation


-Requirements

    CMake ≥ 3.29

    A C++20-capable compiler (clang or gcc)

    x86 CPU with SSE support

    GoogleTest (for unit tests)

-Install dependencies on Debian/Ubuntu:


##bash

sudo apt install cmake clang libgtest-dev

-Building

##bash

git clone <https://github.com/flirz27/MatrixInverser.git>

cd MatrixInverser

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

cmake --build build

-Executable: build/MatrixInverter

-Tests: build/Tests

-Run the tests:

##bash

cd build && ctest --output-on-failure

-Usage

The program reads the matrix dimensions, precision, and entries from the command line:

##bash

./MatrixInverter <N> <M> <precision> <a11> <a12> ... <aNM>

Example — invert a 2×2 matrix with precision 10:

##bash

./MatrixInverter 2 2 10 4 7 2 6