cmake_minimum_required(VERSION 3.10)
project(HuffmanTests)

# Add GoogleTest for testing
add_subdirectory(googletest)

# Include directories for GoogleTest and your Huffman code
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/Huffman_C)

# Automatically find all source files in the Huffman_C directory
file(GLOB HUFFMAN_SOURCES Huffman_C/*.cpp)

# Create a library for the Huffman code
add_library(huffman_lib ${HUFFMAN_SOURCES})

# Create the main executable for the Huffman program
add_executable(huffman_main Huffman_C/huffman_main.cpp)
target_link_libraries(huffman_main huffman_lib)

# Create the test executable
add_executable(huffman_tests tests/huffman_tests.cpp)
target_link_libraries(huffman_tests huffman_lib gtest gtest_main pthread)

# Enable testing and add the tests to CTest
enable_testing()
add_test(NAME HuffmanTests COMMAND huffman_tests)

# Memory leakage check during debug builds
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")