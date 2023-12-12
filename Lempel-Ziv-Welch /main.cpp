#include <iostream>
#include "LZWcompression.cpp"
#include <fstream>
int main() {
    
    // Read input from a text file
    std::ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file.\n";
        return 1;
    }

    string inputContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    // Compression
    vector<int> compressed = compress(inputContent);

    // Write compressed data to a binary file
    std::ofstream compressedFile("compressed.bin", std::ios::binary);
    if (!compressedFile.is_open()) {
        std::cerr << "Error opening compressed file.\n";
        return 1;
    }

    compressedFile.write(reinterpret_cast<const char*>(&compressed[0]), compressed.size() * sizeof(int));
    compressedFile.close();

    // Read compressed data from the binary file
    std::ifstream compressedInputFile("compressed.bin", std::ios::binary);
    if (!compressedInputFile.is_open()) {
        std::cerr << "Error opening compressed input file.\n";
        return 1;
    }

    compressed.clear();
    int code;
    while (compressedInputFile.read(reinterpret_cast<char*>(&code), sizeof(int))) {
        compressed.push_back(code);
    }
    compressedInputFile.close();

    // Decompression
    string decompressed = decompress(compressed);

    // Write decompressed data to a new text file
    std::ofstream decompressedFile("decompressed.txt");
    if (!decompressedFile.is_open()) {
        std::cerr << "Error opening decompressed file.\n";
        return 1;
    }

    decompressedFile << decompressed;
    decompressedFile.close();

    cout << "LZW compression and decompression completed successfully.\n";

    return 0;
}