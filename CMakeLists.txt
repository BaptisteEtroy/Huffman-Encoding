cmake_minimum_required(VERSION 3.10)
project(HuffmanTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories for all builds (Debug, Release, etc.)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find Google Test
find_package(GTest REQUIRED)

# Include directories
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/Huffman_C)

# Create Huffman library
add_library(huffman_lib Huffman_C/huffman.cpp)

# Add the test executable
add_executable(huffman_tests tests/huffman_tests.cpp)
target_link_libraries(huffman_tests 
    huffman_lib 
    GTest::GTest 
    GTest::Main 
    pthread)

# Enable testing
enable_testing()
add_test(NAME HuffmanTests COMMAND huffman_tests)

# Memory leakage check
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")

# Add the main executable
add_executable(main Huffman_C/huffman_main.cpp)
target_link_libraries(main huffman_lib)
