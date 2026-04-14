#include "ARCHIVO.hpp"
template<typename ola>
void guardarArchivo(archivoHeader& header,std::fstream& archivo,ola& objeto,int pos=-1){
    archivo.clear();
    if(pos==-1){
        archivo.seekp(header.cantidadRegistros*sizeof(ola),ios::beg);
    }else{
        archivo.seekp(pos*sizeof(ola),ios::beg);
    }
    archivo.write(reinterpret_cast<char*>(&objeto),sizeof(ola));
    archivo.clear();
    header.cantidadRegistros++;
    header.proximoID++;
}
template<typename ola>
ola cargarArchivo(archivoHeader& header,std::fstream& archivo,int pos){
    ola objeto;
    archivo.clear();
    archivo.seekg(pos*sizeof(ola),ios::beg);
    archivo.read(reinterpret_cast<char*>(&objeto),sizeof(ola));
    archivo.clear();
    return objeto;
}