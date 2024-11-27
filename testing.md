# GoogleTest Integration

This project uses [GoogleTest](https://github.com/google/googletest), a popular C++ testing framework, to ensure the correctness of the Huffman Encoding and Decoding implementation.

## How to Run Tests

1. **Set up the project**:
   ```bash
   # Clone the repository and navigate to the project root
   mkdir build
   cd build

   # Run CMake to configure the project
   cmake ..

   # Build the project
   make

  # Run the test suite
  ./huffman_tests