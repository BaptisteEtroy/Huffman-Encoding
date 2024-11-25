#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <queue>
#include <vector>
#include <cstdint>    // For fixed-size integer types
#include <stdexcept>  // For exception handling

using namespace std;

// Base class Huffman
class Huffman {
public:
    struct Node {
        char data;
        unsigned freq;
        Node *left, *right;
        Node(char data, unsigned freq)
            : data(data), freq(freq), left(nullptr), right(nullptr) {}
        ~Node() {
            delete left;
            delete right;
        }
    };

    Huffman() : root(nullptr) {}

    virtual ~Huffman() {
        delete root;
    }

protected:
    Node* root;
    unordered_map<char, string> huffmanCodes;

    struct Compare {
        bool operator()(Node* left, Node* right) {
            return left->freq > right->freq;
        }
    };

    void generateCodes(Node* node, const string& str) {
        if (!node)
            return;

        if (!node->left && !node->right) {
            huffmanCodes[node->data] = str;
        }

        generateCodes(node->left, str + "0");
        generateCodes(node->right, str + "1");
    }
};

// Compression class derived from Huffman
class Compression : public Huffman {
public:
    bool compress(const string& inputFile, const string& outputFile) {
        ifstream inFile(inputFile, ios::binary | ios::ate);
        if (!inFile.is_open()) {
            cerr << "Error opening input file: " << inputFile << endl;
            return false;
        }

        // Get original file size
        streamsize originalSize = inFile.tellg();
        inFile.seekg(0, ios::beg); // Reset file pointer to beginning

        unordered_map<char, uint64_t> frequency;

        // Read the input file and calculate frequency
        char ch;
        while (inFile.get(ch)) {
            frequency[ch]++;
        }
        inFile.clear();
        inFile.seekg(0, ios::beg); // Reset file pointer to beginning

        // Create priority queue for Huffman tree
        priority_queue<Node*, vector<Node*>, Compare> minHeap;

        // Create leaf nodes
        for (const auto& [character, freq] : frequency) {
            Node* node = new Node(character, freq);
            minHeap.push(node);
        }

        // Build Huffman Tree
        while (minHeap.size() > 1) {
            Node *left = minHeap.top(); minHeap.pop();
            Node *right = minHeap.top(); minHeap.pop();
            Node *newNode = new Node('\0', left->freq + right->freq);
            newNode->left = left;
            newNode->right = right;
            minHeap.push(newNode);
        }

        root = minHeap.top();

        // Generate codes
        generateCodes(root, "");

        // Open the output file to write codebook and compressed data
        ofstream outFile(outputFile, ios::binary);
        if (!outFile.is_open()) {
            cerr << "Error opening output file: " << outputFile << endl;
            return false;
        }

        // Placeholder for codebook size (will update later)
        uint64_t codebookSizePosition = outFile.tellp();
        uint64_t codebookSize = 0;
        outFile.write(reinterpret_cast<char*>(&codebookSize), sizeof(codebookSize));

        // Write the codebook
        uint64_t codeCount = huffmanCodes.size();
        outFile.write(reinterpret_cast<char*>(&codeCount), sizeof(codeCount));

        for (const auto& [character, codeStr] : huffmanCodes) {
            // Write character
            outFile.put(character);

            // Write code length
            uint64_t codeLength = codeStr.size();
            outFile.write(reinterpret_cast<char*>(&codeLength), sizeof(codeLength));

            // Convert code string to bits and write
            uint64_t numFullBytes = codeLength / 8;
            uint64_t remainingBits = codeLength % 8;
            uint64_t totalBytes = numFullBytes + (remainingBits ? 1 : 0);

            vector<unsigned char> codeBytes(totalBytes, 0);

            for (uint64_t i = 0; i < codeLength; ++i) {
                if (codeStr[i] == '1') {
                    codeBytes[i / 8] |= (1 << (7 - (i % 8)));
                }
            }

            // Write the code bytes
            outFile.write(reinterpret_cast<char*>(&codeBytes[0]), totalBytes);
        }

        // Calculate and update the codebook size
        std::streampos currentPosition = outFile.tellp();
        codebookSize = static_cast<uint64_t>(currentPosition) - sizeof(uint64_t);
        outFile.seekp(codebookSizePosition, ios::beg);
        outFile.write(reinterpret_cast<char*>(&codebookSize), sizeof(codebookSize));
        outFile.seekp(0, ios::end); // Return to the end to write compressed data

        // Compress the input file and write to output file
        unsigned char buffer = 0;
        int bitCount = 0;

        while (inFile.get(ch)) {
            const string& code = huffmanCodes[ch];
            for (char bitChar : code) {
                buffer = (buffer << 1) | (bitChar == '1' ? 1 : 0);
                bitCount++;
                if (bitCount == 8) {
                    outFile.put(buffer);
                    buffer = 0;
                    bitCount = 0;
                }
            }
        }

        // Handle remaining bits
        if (bitCount > 0) {
            buffer <<= (8 - bitCount);
            outFile.put(buffer);
        }

        inFile.close();
        outFile.close();

        // Get compressed file size
        ifstream compressedFile(outputFile, ios::binary | ios::ate);
        if (!compressedFile.is_open()) {
            cerr << "Error opening compressed file to get size: " << outputFile << endl;
            return false;
        }
        streamsize compressedSize = compressedFile.tellg();
        compressedFile.close();

        // Calculate and display compression ratio
        double compressionRatio = static_cast<double>(compressedSize) / static_cast<double>(originalSize);
        cout << "Original size: " << originalSize << " bytes" << endl;
        cout << "Compressed size: " << compressedSize << " bytes" << endl;
        cout << "Compression ratio: " << compressionRatio << endl;

        return true;
    }
};

// Decompression class derived from Huffman
class Decompression : public Huffman {
public:
    bool decompress(const string& inputFile, const string& outputFile) {
        // Open the input file to read codebook and compressed data
        ifstream inFile(inputFile, ios::binary);
        if (!inFile.is_open()) {
            cerr << "Error opening input file: " << inputFile << endl;
            return false;
        }

        try {
            // Read the codebook size
            uint64_t codebookSize;
            inFile.read(reinterpret_cast<char*>(&codebookSize), sizeof(codebookSize));
            uint64_t codebookEndPosition = sizeof(uint64_t) + codebookSize;

            // Read the number of codes
            uint64_t codeCount;
            inFile.read(reinterpret_cast<char*>(&codeCount), sizeof(codeCount));

            unordered_map<string, char> reverseHuffmanCodes;

            // Read each character and its code
            for (uint64_t i = 0; i < codeCount; ++i) {
                char ch = inFile.get();

                uint64_t codeLength;
                inFile.read(reinterpret_cast<char*>(&codeLength), sizeof(codeLength));

                if (codeLength > 1024) {
                    throw runtime_error("Code length too large");
                }

                uint64_t numFullBytes = codeLength / 8;
                uint64_t remainingBits = codeLength % 8;
                uint64_t totalBytes = numFullBytes + (remainingBits ? 1 : 0);

                vector<unsigned char> codeBytes(totalBytes);
                inFile.read(reinterpret_cast<char*>(&codeBytes[0]), totalBytes);

                // Convert code bytes to string
                string codeStr;
                for (uint64_t j = 0; j < codeLength; ++j) {
                    unsigned char byte = codeBytes[j / 8];
                    bool bit = (byte >> (7 - (j % 8))) & 1;
                    codeStr += bit ? '1' : '0';
                }

                reverseHuffmanCodes[codeStr] = ch;
            }

            // Open the output file
            ofstream outFile(outputFile, ios::binary);
            if (!outFile.is_open()) {
                cerr << "Error opening output file: " << outputFile << endl;
                return false;
            }

            // Decompress the data
            string currentCode;
            unsigned char byte;

            // Read from the current position after the codebook
            while (inFile.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
                for (int i = 7; i >= 0; --i) {
                    bool bit = (byte >> i) & 1;
                    currentCode += bit ? '1' : '0';
                    auto it = reverseHuffmanCodes.find(currentCode);
                    if (it != reverseHuffmanCodes.end()) {
                        outFile.put(it->second);
                        currentCode.clear();
                    }
                }
            }

            inFile.close();
            outFile.close();

            cout << "Decompression completed successfully." << endl;
            return true;
        } catch (const exception& e) {
            cerr << "Exception during decompression: " << e.what() << endl;
            inFile.close();
            return false;
        }
    }
};

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