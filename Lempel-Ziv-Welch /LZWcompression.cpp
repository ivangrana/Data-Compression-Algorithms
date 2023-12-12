#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

using std::vector;
using std::string;
using std::unordered_map;
using std::cout;    

// Function to perform LZW compression
vector<int> compress(const string &uncompressed) {
    unordered_map<string, int> dictionary;
    int dictSize = 256;

    for (int i = 0; i < 256; ++i) {
        string s = string(1, static_cast<char>(i));
        dictionary[s] = i;
    }

    string current;
    vector<int> result;

    for (char c : uncompressed) {
        string temp = current + c;
        if (dictionary.find(temp) != dictionary.end()) {
            current = temp;
        } else {
            result.push_back(dictionary[current]);
            dictionary[temp] = dictSize++;
            current = string(1, c);
        }
    }

    if (!current.empty()) {
        result.push_back(dictionary[current]);
    }

    return result;
}

// Function to perform LZW decompression
string decompress(const vector<int> &compressed) {
    unordered_map<int, string> dictionary;
    int dictSize = 256;

    for (int i = 0; i < 256; ++i) {
        dictionary[i] = string(1, static_cast<char>(i));
    }

    string current = string(1, static_cast<char>(compressed[0]));
    string result = current;

    for (int i = 1; i < compressed.size(); ++i) {
        string entry;
        if (dictionary.find(compressed[i]) != dictionary.end()) {
            entry = dictionary[compressed[i]];
        } else if (compressed[i] == dictSize) {
            entry = current + current[0];
        } else {
            throw std::runtime_error("Bad compressed sequence");
        }

        result += entry;

        dictionary[dictSize++] = current + entry[0];

        current = entry;
    }

    return result;
}