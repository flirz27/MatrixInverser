A high-performance C++ matrix manipulation library optimized with SSE (Streaming SIMD Extensions). This library provides efficient linear algebra operations by leveraging hardware-level parallelism.

Prerequisites

    CMake (version 3.10 or higher)

    A C++ compiler with SSE support (GCC, Clang, or MSVC)

    Google Test library


Installation
# 1. Create a directory for build artifacts
mkdir build && cd build

# 2. Generate build files 
cmake -DCMAKE_BUILD_TYPE=Release ..

# 3. Compile the project
make

Using
./ MatrixInverter <N> <M> <your matrix> <precision>


