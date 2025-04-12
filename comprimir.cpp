#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <string>
#include <cstdint> //Para uint32_t
#include <vector>
#include <bitset>

#include "arbol.cpp"

using namespace std;

// Función para tomar los códigos binarios (en preorden)
//  Generar códigos Huffman para cada carácter ---
void generateCodes(HuffmanNode* root, const string& code, unordered_map<char, string>& huffmanCodes) {
    if (!root) return;

    // Si llegamos a una hoja (sin hijos), guardar el código
    if (!root->left && !root->right) {
        huffmanCodes[root->ch] = code;
    }

    // Recorrer hijo izquierdo agregando '0' al código actual
    generateCodes(root->left, code + "0", huffmanCodes);

    // Recorrer hijo derecho agregando '1' al código actual
    generateCodes(root->right, code + "1", huffmanCodes);
}
// Serialización del árbol (preorden)
void serializeTree(HuffmanNode* root, string& out) {
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
// Poner bits en bytes, construir con 1 y 0 un byte
vector<unsigned char> bitstringToBytes(const string& bitstring) {
    vector<unsigned char> bytes;
    for (size_t i = 0; i < bitstring.size(); i += 8) {
        string byteStr = bitstring.substr(i, 8);
        while (byteStr.length() < 8) byteStr += '0';  // Padding
        bitset<8> b(byteStr);
        bytes.push_back(static_cast<unsigned char>(b.to_ulong()));
    }
    return bytes;
}

// Codificar el contenido del archivo con los códigos Huffman ---
string encodeText(const string& filename, const unordered_map<char, string>& huffmanCodes) {
    ifstream file(filename, ios::in);
    string encodedText;
    char ch;

    // Leer el archivo y reemplazar cada carácter por su código binario
    while (file.get(ch)) {
        encodedText += huffmanCodes.at(ch);  // Añadir código al texto codificado
    }

    file.close();
    return encodedText;  // Cadena completa de bits como texto
}

// Guardar como archivo visual de texto con bits como caracteres, cofirmar que se guarda digitos binarios
void writeBitstringToTxt(const string& filename, const string& bitstring) {
    ofstream out(filename);
    out << bitstring;  // Guardamos literalmente "0101001001..."
    out.close();
}
void guardarArchivo(string bitstring, string arbol){
    // Guardar archivo comprimido
    vector<unsigned char> bytes = bitstringToBytes(bitstring);
    ofstream out("comprimido.bin", ios::binary);
    uint32_t treeSize = arbol.size();
    out.write(reinterpret_cast<char*>(&treeSize), sizeof(treeSize));
    out.write(arbol.c_str(), treeSize);
    uint32_t bitLength = bitstring.size();
    out.write(reinterpret_cast<char*>(&bitLength), sizeof(bitLength));
    for (unsigned char byte : bytes) {
        out.write(reinterpret_cast<char*>(&byte), 1);
    }
    out.close();

    cout << "Archivo comprimido correctamente como: comprimido.bin\n";
    writeBitstringToTxt("comprimido_visual.txt", bitstring);
}

void comprimir(string nameFile){
    ifstream file(nameFile);
    if (!file) {
        cerr << "No se pudo abrir el archivo de entrada.\n";
        return;
    }
    string text((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    // Conteo de frecuencias
    unordered_map<char, int> freqMap;
    for (char ch : text) freqMap[ch]++;

    // Construcción del árbol y códigos
    HuffmanNode* root = buildTree(freqMap);
    unordered_map<char, string> codes;
    generateCodes(root, "", codes);

    // Codificación
    string bitstring;
    for (char ch : text) bitstring += codes[ch];

    // Serializar árbol
    string treeData;
    serializeTree(root, treeData);

    guardarArchivo(bitstring, treeData);

}