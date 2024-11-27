cmake_minimum_required(VERSION 3.10)

# Project Name
project(HuffmanTests)

# Add GoogleTest
add_subdirectory(googletest)

# Include directories
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/Huffman_C)

# Add the test executable
add_executable(huffman_tests tests/huffman_tests.cpp Huffman_C/huffman.cpp)

# Link libraries
target_link_libraries(huffman_tests gtest gtest_main pthread)

# Enable testing
enable_testing()

# Add tests to CTest
add_test(NAME HuffmanTests COMMAND huffman_tests)

# Memory leakage check
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
