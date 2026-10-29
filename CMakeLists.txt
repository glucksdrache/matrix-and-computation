cmake_minimum_required(VERSION 3.20)
project(matrix_and_computation VERSION 0.1.0 LANGUAGES CXX)

option(MAC_BUILD_TESTS "Build unit tests" ON)
option(MAC_BUILD_BENCHMARKS "Build benchmark programs" OFF)

add_library(mac STATIC
    src/core/matrix.cpp
    src/algorithms/matmul.cpp
)
target_include_directories(mac PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(mac PUBLIC cxx_std_20)
if(MSVC)
    target_compile_options(mac PRIVATE /W4)
else()
    target_compile_options(mac PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(MAC_BUILD_TESTS)
    enable_testing()
    add_executable(test_matrix tests/test_matrix.cpp)
    target_link_libraries(test_matrix PRIVATE mac)
    add_test(NAME matrix_basics COMMAND test_matrix)

    add_executable(test_matmul tests/test_matmul.cpp)
    target_link_libraries(test_matmul PRIVATE mac)
    add_test(NAME matmul COMMAND test_matmul)
endif()

if(MAC_BUILD_BENCHMARKS)
    add_executable(bench_matmul benchmarks/bench_matmul.cpp)
    target_link_libraries(bench_matmul PRIVATE mac)
endif()
