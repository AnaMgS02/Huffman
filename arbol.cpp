#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <string>
#include <cstdint> //Para uint32_t
#include <vector>
#include <bitset>

using namespace std;

//Almacenar las frecuencias de cada caracter con 'unordered_map<char, int>' clave:valor, similar al diccionario
unordered_map<char, int> countFrequencies(const string& filename) {
    unordered_map<char, int> freq;
    ifstream file(filename, ios::in);// Abrir archivo en modo lectura

    if (!file.is_open()) {//Si no se logra abrir el archivo 
        cerr << "No se pudo abrir el archivo: " << filename << endl;
        return freq;
    }

    //Para leer cada caracter del archivo
    char ch;
    while (file.get(ch)) {
        freq[ch]++;
    }

    file.close();
    return freq;
}

//Estructura de cada nodo para árbol de Huffman
struct HuffmanNode {
    char ch;               // Carácter (si es hoja)
    int freq;              // Frecuencia del carácter
    HuffmanNode* left;     // Hijo izquierdo
    HuffmanNode* right;    // Hijo derecho

    HuffmanNode(char c, int f)
        :ch(c), freq(f), left(nullptr), right(nullptr) {}//Constructor
};

// Comparador para organizar por prioridad
// Los nodos con menor frecuencia tienen mayor prioridad
struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->freq > b->freq;
    }
};

// Construcción del árbol de Huffman a partir de las frecuencias
HuffmanNode* buildTree(const unordered_map<char, int>& freqMap) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;

    // Crear un nodo hoja por cada carácter y añadirlo a la cola de prioridad
    for (const auto& pair : freqMap) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    // Combinar nodos hasta que quede solo uno (raíz del árbol)
    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();

        // Crear un nuevo nodo interno con frecuencia total
        HuffmanNode* merged = new HuffmanNode('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;

        pq.push(merged);
    }

    return pq.top();  // Raíz del árbol
}


void printFrequencies(const unordered_map<char, int>& freq) {
    cout << "Frecuencias de caracteres:\n";
    for (const auto& pair : freq) {
        if (pair.first == '\n') {
            cout << "\\n: " << pair.second << "\n";
        } else if (pair.first == ' ') {
            cout << "' ': " << pair.second << "\n";
        } else {
            cout << pair.first << ": " << pair.second << "\n";
        }
    }
}

