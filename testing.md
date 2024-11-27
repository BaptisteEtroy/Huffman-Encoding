# GoogleTest Integration

This project uses [GoogleTest](https://github.com/google/googletest), a popular C++ testing framework, to ensure the correctness of the Huffman Encoding and Decoding implementation.

## How to Run Tests
1. **Downloading Google test**:
   ```bash
   # Clone the Google Test repository
   git clone https://github.com/google/googletest.git


2. **Set up the project**:
   ```bash
   # Clone the repository and navigate to the project root
   mkdir build
   cd build

   # Run CMake to configure the project
   cmake ..

   # Build the project
   make

  # Run the test suite
  ```bash
  ./huffman_tests

## Google Test Output
   ```bash
   [==========] Running 5 tests from 1 test suite.
   [----------] Global test environment set-up.
   [----------] 5 tests from HuffmanTests/HuffmanEncodingTest
   [ RUN      ] HuffmanTests/HuffmanEncodingTest.CompressionAndDecompression/2
   Original size: 4357 bytes
   Compressed size: 3014 bytes
   Compression ratio: 0.69176
   Decompression completed successfully.
   [       OK ] HuffmanTests/HuffmanEncodingTest.CompressionAndDecompression/2 (2 ms)
   [ RUN      ] HuffmanTests/HuffmanEncodingTest.CompressionAndDecompression/3
   Original size: 24530 bytes
   Compressed size: 13211 bytes
   Compression ratio: 0.538565
   Decompression completed successfully.
   [       OK ] HuffmanTests/HuffmanEncodingTest.CompressionAndDecompression/3 (44 ms)
   [ RUN      ] HuffmanTests/HuffmanEncodingTest.CompressionAndDecompression/4
   Original size: 43570 bytes
   Compressed size: 29014 bytes
   Compression ratio: 0.66753
   Decompression completed successfully.
   [       OK ] HuffmanTests/HuffmanEncodingTest.CompressionAndDecompression/4 (207 ms)
   [----------] 5 tests from HuffmanTests/HuffmanEncodingTest (260 ms total)

   [----------] Global test environment tear-down
   [==========] 5 tests from 1 test suite ran. (260 ms total)
   [  PASSED  ] 5 tests.