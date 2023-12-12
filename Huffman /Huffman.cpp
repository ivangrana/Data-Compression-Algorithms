#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

// Define a structure to represent a node in the Huffman tree
struct HuffmanNode {
    char data;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char data, int frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
};

// Comparison function for priority queue to build the Huffman tree
struct Compare {
    bool operator()(HuffmanNode* left, HuffmanNode* right) {
        return left->frequency > right->frequency;
    }
};

// Traverse the Huffman tree and build codes
void buildCodes(HuffmanNode* root, string code, unordered_map<char, string>& codes) {
    if (root) {
        if (!root->left && !root->right) {
            codes[root->data] = code;
        }
        buildCodes(root->left, code + "0", codes);
        buildCodes(root->right, code + "1", codes);
    }
}

// Build the Huffman tree and return the root
HuffmanNode* buildHuffmanTree(const unordered_map<char, int>& frequencyMap) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> minHeap;

    // Create a leaf node for each character and add it to the min heap
    for (const auto& pair : frequencyMap) {
        minHeap.push(new HuffmanNode(pair.first, pair.second));
    }

    // Build the Huffman tree
    while (minHeap.size() > 1) {
        HuffmanNode* left = minHeap.top();
        minHeap.pop();

        HuffmanNode* right = minHeap.top();
        minHeap.pop();

        HuffmanNode* internalNode = new HuffmanNode('\0', left->frequency + right->frequency);
        internalNode->left = left;
        internalNode->right = right;

        minHeap.push(internalNode);
    }

    return minHeap.top();
}

// Compress the input text using Huffman coding
string compress(const string& text, unordered_map<char, string>& codes) {
    unordered_map<char, int> frequencyMap;

    // Count the frequency of each character in the text
    for (char c : text) {
        frequencyMap[c]++;
    }

    // Build the Huffman tree
    HuffmanNode* root = buildHuffmanTree(frequencyMap);

    // Build the Huffman codes
    buildCodes(root, "", codes);

    // Encode the text using Huffman codes
    string encodedText;
    for (char c : text) {
        encodedText += codes[c];
    }

    return encodedText;
}

int main() {
    // Read input text from a file
    ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open input file." << endl;
        return 1;
    }

    string inputText((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    inputFile.close();

    // Compress the input text and get the dictionary table
    unordered_map<char, string> codes;
    string compressedText = compress(inputText, codes);

    // Write the compressed text to an output file
    ofstream outputFile("compressed_output.bin");
    if (!outputFile.is_open()) {
        cerr << "Error: Could not open output file." << endl;
        return 1;
    }

    // Output the dictionary table
    cout << "Dictionary Table:" << endl;
    for (const auto& pair : codes) {
        cout << pair.first << ": " << pair.second << endl;
    }

    // Output the compressed text
    outputFile << compressedText;
    outputFile.close();

    cout << "\nCompression successful. Compressed output written to compressed_output.txt." << endl;

    return 0;
}
