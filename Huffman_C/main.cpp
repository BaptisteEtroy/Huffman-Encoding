#include "huffman.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " [c|d] input_file output_file" << endl;
        return 1;
    }

    string mode = argv[1];
    string inputFile = argv[2];
    string outputFile = argv[3];

    if (mode == "c") {
        Compression compressor;
        if (!compressor.compress(inputFile, outputFile)) {
            cerr << "Compression failed!" << endl;
            return 1;
        } else {
            cout << "Compression completed successfully." << endl;
        }
    } else if (mode == "d") {
        Decompression decompressor;
        if (!decompressor.decompress(inputFile, outputFile)) {
            cerr << "Decompression failed!" << endl;
            return 1;
        } else {
            cout << "Decompression completed successfully." << endl;
        }
    } else {
        cerr << "Invalid mode. Use 'c' for compression or 'd' for decompression." << endl;
        return 1;
    }

    return 0;
}