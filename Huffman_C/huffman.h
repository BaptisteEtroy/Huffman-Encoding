#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>
#include <unordered_map>
#include <queue>
#include <vector>
#include <cstdint> // For fixed-size integer types

class PoolAllocator {
public:
    PoolAllocator(size_t objectSize, size_t blockSize = 1024);
    ~PoolAllocator();

    void* allocate();
    void deallocate(void* ptr);

private:
    struct Block {
        Block* next;
    };

    void allocateBlock();

    size_t objectSize_;
    size_t blockSize_;
    Block* freeList_;
    std::vector<void*> blocks_;
};

class Huffman {
public:
    struct Node {
        char data;
        unsigned freq;
        Node* left;
        Node* right;

        Node(char data, unsigned freq);
        ~Node();

        void* operator new(size_t size);
        void operator delete(void* ptr);
    };

    Huffman();
    virtual ~Huffman();

protected:
    Node* root;
    std::unordered_map<char, std::string> huffmanCodes;

    struct Compare {
        bool operator()(Node* left, Node* right);
    };

    void generateCodes(Node* node, const std::string& str);

private:
    static PoolAllocator nodePoolAllocator_;
};

class Compression : public Huffman {
public:
    bool compress(const std::string& inputFile, const std::string& outputFile);
};

class Decompression : public Huffman {
public:
    bool decompress(const std::string& inputFile, const std::string& outputFile);
};

#endif // HUFFMAN_H
