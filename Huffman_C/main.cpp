#include "huffman.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cerr << "Error: Invalid arguments provided.\n"
             << endl;
        cerr << "Usage: " << argv[0] << " [c|d] input_file output_file" << endl;
        return 1;
    }

    string mode = argv[1];
    string inputFile = argv[2];
    string outputFile = argv[3];

    try
    {
        if (mode == "c")
        {
            Compression compressor;
            if (!compressor.compress(inputFile, outputFile))
            {
                throw runtime_error("Compression failed!");
            }
            else
            {
                cout << "Compression completed successfully." << endl;
            }
        }
        else if (mode == "d")
        {
            Decompression decompressor;
            if (!decompressor.decompress(inputFile, outputFile))
            {
                throw runtime_error("Decompression failed!");
            }
            else
            {
                cout << "Decompression completed successfully." << endl;
            }
        }
        else 
        {
            throw invalid_argument("Invalid mode. Use 'c' for compression or 'd' for decompression.");
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}