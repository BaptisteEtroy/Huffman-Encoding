cmake_minimum_required(VERSION 3.10)
project(HuffmanTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add GoogleTest
add_subdirectory(googletest)

# Include directories
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/Huffman_C)

# Create Huffman library
add_library(huffman_lib Huffman_C/huffman.cpp)

# Add the test executable
add_executable(huffman_tests tests/huffman_tests.cpp)
target_link_libraries(huffman_tests huffman_lib gtest gtest_main pthread)

# Enable testing
enable_testing()
add_test(NAME HuffmanTests COMMAND huffman_tests)

# Memory leakage check
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")

# Add the main executable
add_executable(main Huffman_C/huffman_main.cpp)
target_link_libraries(main huffman_lib)
