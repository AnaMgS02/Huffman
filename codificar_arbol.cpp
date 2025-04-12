#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <string>

#include "frecuencias.cpp"
#include "arbol.cpp"

using namespace std;

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

// --- Programa principal para probar lo hecho ---
int main() {
    string filename = "cancion.txt";  // Archivo de entrada

    // Contar frecuencias de caracteres
    auto freqMap = countFrequencies(filename);

    // Construir árbol de Huffman
    HuffmanNode* root = buildHuffmanTree(freqMap);

    // Generar códigos Huffman
    unordered_map<char, string> huffmanCodes;
    generateCodes(root, "", huffmanCodes);

    // Mostrar códigos Huffman por pantalla
    cout << "Códigos Huffman generados:\n";
    for (const auto& pair : huffmanCodes) {
        if (pair.first == '\n')
            cout << "'\\n': " << pair.second << "\n";
        else if (pair.first == ' ')
            cout << "' ': " << pair.second << "\n";
        else
            cout << "'" << pair.first << "': " << pair.second << "\n";
    }

    // Codificar el archivo de entrada usando los códigos generados
    string encoded = encodeText(filename, huffmanCodes);

    // Mostrar los primeros 100 bits del texto codificado (para no saturar la consola)
    cout << "\nTexto codificado (primeros 100 bits):\n";
    cout << encoded.substr(0, 100) << "...\n";

    // Mostrar cuántos bits tiene en total la versión comprimida (sin convertir a binario aún)
    cout << "\nTotal de bits: " << encoded.length() << "\n";

    return 0;
}
