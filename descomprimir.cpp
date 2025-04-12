#include <cstdint> //Para uint32_t
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>

struct HuffmanNode {
    char ch;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char c = '\0') : ch(c), left(nullptr), right(nullptr) {}
};

// Reconstruir árbol desde serialización
HuffmanNode* deserializeTree(const std::string& data, size_t& index) {
    if (index >= data.size()) return nullptr;

    char flag = data[index++];
    if (flag == '1') {
        char ch = data[index++];
        return new HuffmanNode(ch);
    } else {
        HuffmanNode* node = new HuffmanNode();
        node->left = deserializeTree(data, index);
        node->right = deserializeTree(data, index);
        return node;
    }
}

std::string readBitsFromBytes(const std::vector<unsigned char>& bytes, uint32_t totalBits) {
    std::string bitstring;
    for (unsigned char byte : bytes) {
        bitstring += std::bitset<8>(byte).to_string();
    }
    return bitstring.substr(0, totalBits);
}

std::string decodeText(HuffmanNode* root, const std::string& bitstring) {
    std::string result;
    HuffmanNode* current = root;
    for (char bit : bitstring) {
        if (bit == '0') current = current->left;
        else current = current->right;

        if (!current->left && !current->right) {
            result += current->ch;
            current = root;
        }
    }
    return result;
}

int main() {
    std::string inputFile;
    std::cout << "Ingrese el nombre del archivo .bin a descomprimir: ";
    std::getline(std::cin, inputFile);

    std::ifstream file(inputFile, std::ios::binary);
    if (!file) {
        std::cerr << "No se pudo abrir el archivo binario.\n";
        return 1;
    }

    uint32_t treeSize;
    file.read(reinterpret_cast<char*>(&treeSize), sizeof(treeSize));
    std::string treeData(treeSize, '\0');
    file.read(&treeData[0], treeSize);

    uint32_t bitLength;
    file.read(reinterpret_cast<char*>(&bitLength), sizeof(bitLength));

    std::vector<unsigned char> encodedBytes;
    while (!file.eof()) {
        unsigned char byte;
        file.read(reinterpret_cast<char*>(&byte), 1);
        if (file.gcount() > 0) {
            encodedBytes.push_back(byte);
        }
    }
    file.close();

    size_t idx = 0;
    HuffmanNode* root = deserializeTree(treeData, idx);
    std::string bitstring = readBitsFromBytes(encodedBytes, bitLength);
    std::string decodedText = decodeText(root, bitstring);

    std::ofstream out("descomprimido.txt");
    out << decodedText;
    out.close();

    std::cout << "Archivo descomprimido correctamente como: descomprimido.txt\n";
    return 0;
}
