#include "huffman.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <stdexcept> // For exception handling

using namespace std;

// PoolAllocator Implementation
PoolAllocator::PoolAllocator(size_t objectSize, size_t blockSize)
    : objectSize_(objectSize), blockSize_(blockSize), freeList_(nullptr) {}

PoolAllocator::~PoolAllocator() {
    for (void* block : blocks_) {
        ::operator delete(block);
    }
}

void PoolAllocator::allocateBlock() {
    size_t size = blockSize_ * objectSize_;
    char* block = static_cast<char*>(::operator new(size));
    blocks_.push_back(block);
    // Add all blocks to the freelist
    for (size_t i = 0; i < blockSize_; ++i) {
        Block* node = reinterpret_cast<Block*>(block + i * objectSize_);
        node->next = freeList_;
        freeList_ = node;
    }
}

void* PoolAllocator::allocate() {
    if (!freeList_) {
        allocateBlock();
    }
    Block* head = freeList_;
    freeList_ = freeList_->next;
    return head;
}

void PoolAllocator::deallocate(void* ptr) {
    Block* node = static_cast<Block*>(ptr);
    node->next = freeList_;
    freeList_ = node;
}

// Initialize the static PoolAllocator instance
PoolAllocator Huffman::nodePoolAllocator_(sizeof(Huffman::Node));

// Huffman::Node Implementation
Huffman::Node::Node(char data, unsigned freq)
    : data(data), freq(freq), left(nullptr), right(nullptr) {}

Huffman::Node::~Node() {
    delete left;
    delete right;
}

void* Huffman::Node::operator new(size_t size) {
    return Huffman::nodePoolAllocator_.allocate();
}

void Huffman::Node::operator delete(void* ptr) {
    Huffman::nodePoolAllocator_.deallocate(ptr);
}

// Huffman Implementation
Huffman::Huffman() : root(nullptr) {}

Huffman::~Huffman() {
    delete root;
}

bool Huffman::Compare::operator()(Node* left, Node* right) {
    return left->freq > right->freq;
}

void Huffman::generateCodes(Node* node, const string& str) {
    if (!node)
        return;

    if (!node->left && !node->right) {
        huffmanCodes[node->data] = str;
    }

    generateCodes(node->left, str + "0");
    generateCodes(node->right, str + "1");
}

// Compression Implementation
bool Compression::compress(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile, ios::binary | ios::ate);
    if (!inFile.is_open()) {
        cerr << "Error opening input file: " << inputFile << endl;
        return false;
    }

    streamsize originalSize = inFile.tellg();
    inFile.seekg(0, ios::beg);

    unordered_map<char, uint64_t> frequency;
    char ch;
    while (inFile.get(ch)) {
        frequency[ch]++;
    }
    inFile.clear();
    inFile.seekg(0, ios::beg);

    priority_queue<Node*, vector<Node*>, Compare> minHeap;
    for (const auto& [character, freq] : frequency) {
        Node* node = new Node(character, freq);
        minHeap.push(node);
    }

    while (minHeap.size() > 1) {
        Node* left = minHeap.top();
        minHeap.pop();
        Node* right = minHeap.top();
        minHeap.pop();
        Node* newNode = new Node('\0', left->freq + right->freq);
        newNode->left = left;
        newNode->right = right;
        minHeap.push(newNode);
    }

    root = minHeap.top();
    generateCodes(root, "");

    ofstream outFile(outputFile, ios::binary);
    if (!outFile.is_open()) {
        cerr << "Error opening output file: " << outputFile << endl;
        return false;
    }

    uint64_t codebookSizePosition = outFile.tellp();
    uint64_t codebookSize = 0;
    outFile.write(reinterpret_cast<char*>(&codebookSize), sizeof(codebookSize));

    uint64_t codeCount = huffmanCodes.size();
    outFile.write(reinterpret_cast<char*>(&codeCount), sizeof(codeCount));

    for (const auto& [character, codeStr] : huffmanCodes) {
        outFile.put(character);

        uint64_t codeLength = codeStr.size();
        outFile.write(reinterpret_cast<char*>(&codeLength), sizeof(codeLength));

        uint64_t numFullBytes = codeLength / 8;
        uint64_t remainingBits = codeLength % 8;
        uint64_t totalBytes = numFullBytes + (remainingBits ? 1 : 0);

        vector<unsigned char> codeBytes(totalBytes, 0);

        for (uint64_t i = 0; i < codeLength; ++i) {
            if (codeStr[i] == '1') {
                codeBytes[i / 8] |= (1 << (7 - (i % 8)));
            }
        }

        outFile.write(reinterpret_cast<char*>(&codeBytes[0]), totalBytes);
    }

    std::streampos currentPosition = outFile.tellp();
    codebookSize = static_cast<uint64_t>(currentPosition) - sizeof(uint64_t);
    outFile.seekp(codebookSizePosition, ios::beg);
    outFile.write(reinterpret_cast<char*>(&codebookSize), sizeof(codebookSize));
    outFile.seekp(0, ios::end);

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

    if (bitCount > 0) {
        buffer <<= (8 - bitCount);
        outFile.put(buffer);
    }

    inFile.close();
    outFile.close();

    ifstream compressedFile(outputFile, ios::binary | ios::ate);
    if (!compressedFile.is_open()) {
        cerr << "Error opening compressed file to get size: " << outputFile << endl;
        return false;
    }
    streamsize compressedSize = compressedFile.tellg();
    compressedFile.close();

    double compressionRatio = static_cast<double>(compressedSize) / static_cast<double>(originalSize);
    cout << "Original size: " << originalSize << " bytes" << endl;
    cout << "Compressed size: " << compressedSize << " bytes" << endl;
    cout << "Compression ratio: " << compressionRatio << endl;

    return true;
}

// Decompression Implementation
bool Decompression::decompress(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile, ios::binary);
    if (!inFile.is_open()) {
        cerr << "Error opening input file: " << inputFile << endl;
        return false;
    }

    try {
        uint64_t codebookSize;
        inFile.read(reinterpret_cast<char*>(&codebookSize), sizeof(codebookSize));
        uint64_t codebookEndPosition = sizeof(uint64_t) + codebookSize;

        uint64_t codeCount;
        inFile.read(reinterpret_cast<char*>(&codeCount), sizeof(codeCount));

        unordered_map<string, char> reverseHuffmanCodes;

        for (uint64_t i = 0; i < codeCount; ++i) {
            char ch = inFile.get();

            uint64_t codeLength;
            inFile.read(reinterpret_cast<char*>(&codeLength), sizeof(codeLength));

            uint64_t numFullBytes = codeLength / 8;
            uint64_t remainingBits = codeLength % 8;
            uint64_t totalBytes = numFullBytes + (remainingBits ? 1 : 0);

            vector<unsigned char> codeBytes(totalBytes);
            inFile.read(reinterpret_cast<char*>(&codeBytes[0]), totalBytes);

            string codeStr;
            for (uint64_t j = 0; j < codeLength; ++j) {
                unsigned char byte = codeBytes[j / 8];
                bool bit = (byte >> (7 - (j % 8))) & 1;
                codeStr += bit ? '1' : '0';
            }

            reverseHuffmanCodes[codeStr] = ch;
        }

        ofstream outFile(outputFile, ios::binary);
        if (!outFile.is_open()) {
            cerr << "Error opening output file: " << outputFile << endl;
            return false;
        }

        string currentCode;
        unsigned char byte;

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
