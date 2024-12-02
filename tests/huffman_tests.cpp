#include <gtest/gtest.h>
#include "../Huffman_C/huffman.h" // Your Huffman implementation

// List of test case files
const std::vector<std::string> testFiles = {
    "../tests/Test Cases/simple_phrase.txt",
    "../tests/Test Cases/numerical.txt",
    "../tests/Test Cases/repeated_char.txt",
    "../tests/Test Cases/multiple_lines.txt",
    "../tests/Test Cases/large_file.txt"
};

// Parameterized test for Huffman Encoding
class HuffmanEncodingTest : public ::testing::TestWithParam<std::string> {};

TEST_P(HuffmanEncodingTest, CompressionAndDecompression) {
    std::string inputFile = GetParam();
    std::string compressedFile = "../compressed.bin";
    std::string outputFile = "../output.txt";

    // Compress the input file
    Compression compressor;
    ASSERT_TRUE(compressor.compress(inputFile, compressedFile)) << "Failed to compress: " << inputFile;

    // Decompress the compressed file
    Decompression decompressor;
    ASSERT_TRUE(decompressor.decompress(compressedFile, outputFile)) << "Failed to decompress: " << compressedFile;
}

// Instantiate tests for all test case files
INSTANTIATE_TEST_SUITE_P(HuffmanTests, HuffmanEncodingTest, ::testing::ValuesIn(testFiles));

/* 
    * The following test deliberate Memory Issue, like a buffer overflow
    * to test out that google test has memory leak detection
    
TEST(HuffmanEncoding, MemoryTest) {
    int* arr = new int[5];
    arr[5] = 10;  // Buffer overflow
    delete[] arr;
}
*/