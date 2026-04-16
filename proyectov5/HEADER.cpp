#include "HEADER.hpp"
#include <iostream>
#include <fstream>
using namespace std;
void archivoHeader::actualizarheader(archivoHeader header,std::fstream& archivo){
    int x=0;
    if(!archivo.is_open()){
        cout<<"No se pudo abrir el archivo"<<endl;
        return;
    }
        archivo.clear();
        archivo.seekg(0,ios::beg);
        archivo.write(reinterpret_cast<char*>(&header),sizeof(archivoHeader));
        archivo.clear();

}   