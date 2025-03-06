#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <bitset>
#include <map>
#include <functional>

using namespace std;

// Huffman Tree Node
struct HuffmanNode {
    char data;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char data, int freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}

    ~HuffmanNode() {
        delete left;
        delete right;
    }
};

// Comparator for priority queue
struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->freq > b->freq;
    }
};

// DataSculptor Class
class DataSculptor {
private:
    HuffmanNode* root;
    unordered_map<char, string> huffmanCodes;
    unordered_map<string, char> reverseMapping;
    map<string, map<char, int>> markovModel;

    // Build Huffman Tree
    HuffmanNode* buildHuffmanTree(const unordered_map<char, int>& freqMap) {
        priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> minHeap;

        for (auto& pair : freqMap) {
            minHeap.push(new HuffmanNode(pair.first, pair.second));
        }

        while (minHeap.size() > 1) {
            HuffmanNode* left = minHeap.top(); minHeap.pop();
            HuffmanNode* right = minHeap.top(); minHeap.pop();
            HuffmanNode* merged = new HuffmanNode('\0', left->freq + right->freq);
            merged->left = left;
            merged->right = right;
            minHeap.push(merged);
        }

        return minHeap.top();
    }

    // Generate Huffman Codes
    void buildHuffmanCodes(HuffmanNode* node, string code) {
        if (!node) return;

        if (node->data != '\0') {
            huffmanCodes[node->data] = code;
            reverseMapping[code] = node->data;
        }

        buildHuffmanCodes(node->left, code + "0");
        buildHuffmanCodes(node->right, code + "1");
    }

    // Train Markov Model (for text files only)
    void trainMarkovModel(const string& data, int order) {
        for (size_t i = 0; i < data.size() - order; ++i) {
            string state = data.substr(i, order);
            char nextChar = data[i + order];
            markovModel[state][nextChar]++;
        }
    }

    // Predict Next Character using Markov Model (for text files only)
    char predictNextChar(const string& state) {
        if (markovModel.find(state) != markovModel.end()) {
            auto& nextCharCounter = markovModel[state];
            char bestChar = '\0';
            int maxCount = 0;

            for (auto& pair : nextCharCounter) {
                if (pair.second > maxCount) {
                    maxCount = pair.second;
                    bestChar = pair.first;
                }
            }
            return bestChar;
        }
        return '\0';
    }

public:
    DataSculptor() : root(nullptr) {}

    ~DataSculptor() {
        delete root;
    }

    // Adaptive Huffman Encoding
    string adaptiveHuffman(const string& data, int order = 2) {
        if (order > 0) {
            trainMarkovModel(data, order); // Train Markov model for text files
        }

        // Build frequency map
        unordered_map<char, int> freqMap;
        for (char ch : data) {
            freqMap[ch]++;
        }

        // Build Huffman Tree and Codes
        root = buildHuffmanTree(freqMap);
        buildHuffmanCodes(root, "");

        // Encode data
        string encodedData;
        for (char ch : data) {
            encodedData += huffmanCodes[ch];
        }

        return encodedData;
    }

    // Decode data
    string decodeData(const string& encodedData) {
        string currentCode;
        string decodedData;

        for (char bit : encodedData) {
            currentCode += bit;
            if (reverseMapping.find(currentCode) != reverseMapping.end()) {
                decodedData += reverseMapping[currentCode];
                currentCode.clear();
            }
        }

        return decodedData;
    }

    // Compress file (handles both text and binary files)
    void compress(const string& inputFile, const string& outputFile) {
        ifstream inFile(inputFile, ios::in | ios::binary);
        if (!inFile) {
            cerr << "Error opening input file!" << endl;
            return;
        }

        // Read file data
        string data((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
        inFile.close();

        // Encode data
        string encodedData = adaptiveHuffman(data, 0); // Disable Markov model for binary files

        // Write compressed data to binary file
        ofstream outFile(outputFile, ios::out | ios::binary);
        if (!outFile) {
            cerr << "Error opening output file!" << endl;
            return;
        }

        // Pad encoded data to make its length a multiple of 8
        int padding = 8 - (encodedData.size() % 8);
        encodedData += string(padding, '0');

        // Write padding info and encoded data
        outFile.put(static_cast<char>(padding));
        for (size_t i = 0; i < encodedData.size(); i += 8) {
            string byteStr = encodedData.substr(i, 8);
            bitset<8> bits(byteStr);
            outFile.put(static_cast<char>(bits.to_ulong()));
        }

        outFile.close();
        cout << "File compressed to " << outputFile << endl;
    }

    // Decompress file (handles both text and binary files)
    void decompress(const string& inputFile, const string& outputFile) {
        ifstream inFile(inputFile, ios::in | ios::binary);
        if (!inFile) {
            cerr << "Error opening input file!" << endl;
            return;
        }

        // Read padding info
        char paddingChar;
        inFile.get(paddingChar);
        int padding = static_cast<int>(paddingChar);

        // Read encoded data
        string encodedData;
        char byte;
        while (inFile.get(byte)) {
            encodedData += bitset<8>(byte).to_string();
        }
        inFile.close();

        // Remove padding
        encodedData = encodedData.substr(0, encodedData.size() - padding);

        // Decode data
        string decodedData = decodeData(encodedData);

        // Write decompressed data to output file
        ofstream outFile(outputFile, ios::out | ios::binary);
        if (!outFile) {
            cerr << "Error opening output file!" << endl;
            return;
        }

        outFile.write(decodedData.c_str(), decodedData.size());
        outFile.close();
        cout << "File decompressed to " << outputFile << endl;
    }
};

// Main Function
int main() {
    DataSculptor ds;

    // Compress a file
    string inputFile = "example.jpg"; // Replace with your image file
    string compressedFile = "compressed.bin";
    string decompressedFile = "decompressed.jpg";

    ds.compress(inputFile, compressedFile);

    // Decompress the file
    ds.decompress(compressedFile, decompressedFile);

    return 0;
}