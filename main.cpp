#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <string>
#include <cstdint> //Para uint32_t
#include <vector>
#include <bitset>

#include "comprimir.cpp"//Fucniones para comprimir

using namespace std;

int main() {
    int opcion1=0;
    string namefile="";
    do{
        cout << "1. Comprimir archivo\n2. Descomprimir archivo\n3. Salir\nOpción: ";
        cin>>opcion1;
        switch (opcion1)
        {
        case 1:
            cout<<"COMPRIMIR ARCHIVO\nIngrese el nombre del archivo a comprimir";
            cin>>namefile;
            comprimir(namefile);
            break;
        case 2:
            /* code */
            break;
        case 3:
            /* code */
            break;
        default:
            cout<<"La opción ingresada no es válida";
            break;
        }
    }while(opcion1!=3);

    return 0;
}
