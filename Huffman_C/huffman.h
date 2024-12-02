#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>
#include <unordered_map>
#include <queue>
#include <vector>
#include <cstdint> // For fixed-size integer types

class Huffman {
public:
    struct Node {
        char data;
        unsigned freq;
        Node *left, *right;
        Node(char data, unsigned freq);
        ~Node();
    };

    Huffman();
    virtual ~Huffman();

protected:
    Node *root;
    std::unordered_map<char, std::string> huffmanCodes;

    struct Compare {
        bool operator()(Node *left, Node *right);
    };

    void generateCodes(Node *node, const std::string &str);
};

class Compression : public Huffman {
public:
    bool compress(const std::string &inputFile, const std::string &outputFile);
};

class Decompression : public Huffman {
public:
    bool decompress(const std::string &inputFile, const std::string &outputFile);
};

#endif // HUFFMAN_H