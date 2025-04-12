#include <cstdint> //Para uint32_t
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <queue>
#include <vector>
#include <bitset>

struct HuffmanNode {
    char ch;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// Comparador para priority_queue
struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->freq > b->freq;
    }
};

// Construcción del árbol de Huffman
HuffmanNode* buildTree(const std::unordered_map<char, int>& freqMap) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Compare> pq;
    for (auto& pair : freqMap) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();

        HuffmanNode* merged = new HuffmanNode('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;
        pq.push(merged);
    }

    return pq.top();
}

// Generar códigos de Huffman
void generateCodes(HuffmanNode* root, std::string current, std::unordered_map<char, std::string>& codes) {
    if (!root) return;
    if (!root->left && !root->right) {
        codes[root->ch] = current;
    }
    generateCodes(root->left, current + "0", codes);
    generateCodes(root->right, current + "1", codes);
}

// Serialización del árbol (preorden)
void serializeTree(HuffmanNode* root, std::string& out) {
    if (!root) return;
    if (!root->left && !root->right) {
        out += '1';
        out += root->ch;
    } else {
        out += '0';
        serializeTree(root->left, out);
        serializeTree(root->right, out);
    }
}

// Empaquetar bits en bytes
std::vector<unsigned char> bitstringToBytes(const std::string& bitstring) {
    std::vector<unsigned char> bytes;
    for (size_t i = 0; i < bitstring.size(); i += 8) {
        std::string byteStr = bitstring.substr(i, 8);
        while (byteStr.length() < 8) byteStr += '0';  // Padding
        std::bitset<8> b(byteStr);
        bytes.push_back(static_cast<unsigned char>(b.to_ulong()));
    }
    return bytes;
}

// Guardar como archivo visual de texto con bits como caracteres
void writeBitstringToTxt(const std::string& filename, const std::string& bitstring) {
    std::ofstream out(filename);
    out << bitstring;  // Guardamos literalmente "0101001001..."
    out.close();
}


int main() {
    std::string inputFile;
    std::cout << "Ingrese el nombre del archivo .txt a comprimir: ";
    std::getline(std::cin, inputFile);

    std::ifstream file(inputFile);
    if (!file) {
        std::cerr << "No se pudo abrir el archivo de entrada.\n";
        return 1;
    }

    std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Conteo de frecuencias
    std::unordered_map<char, int> freqMap;
    for (char ch : text) freqMap[ch]++;

    // Construcción del árbol y códigos
    HuffmanNode* root = buildTree(freqMap);
    std::unordered_map<char, std::string> codes;
    generateCodes(root, "", codes);

    // Codificación
    std::string bitstring;
    for (char ch : text) bitstring += codes[ch];

    // Serializar árbol
    std::string treeData;
    serializeTree(root, treeData);

    // Guardar archivo comprimido
    std::vector<unsigned char> bytes = bitstringToBytes(bitstring);
    std::ofstream out("comprimido.bin", std::ios::binary);
    uint32_t treeSize = treeData.size();
    out.write(reinterpret_cast<char*>(&treeSize), sizeof(treeSize));
    out.write(treeData.c_str(), treeSize);
    uint32_t bitLength = bitstring.size();
    out.write(reinterpret_cast<char*>(&bitLength), sizeof(bitLength));
    for (unsigned char byte : bytes) {
        out.write(reinterpret_cast<char*>(&byte), 1);
    }
    out.close();

    std::cout << "Archivo comprimido correctamente como: comprimido.bin\n";
    writeBitstringToTxt("comprimido_visual.txt", bitstring);
    return 0;
}
