#include "ARCHIVO.hpp"
template<typename ola>
void guardarArchivo(archivoHeader& header,std::fstream& archivo,ola& objeto,int pos){
    archivo.clear();
    if(pos==-1){
        archivo.seekp(header.cantidadRegistros*sizeof(ola),std::ios::beg);
    }else{
        archivo.seekp(pos*sizeof(ola),std::ios::beg);
    }
    archivo.write(reinterpret_cast<char*>(&objeto),sizeof(ola));
    archivo.clear();
    header.cantidadRegistros++;
    header.proximoID++;
}
template<typename templeatcito>
templeatcito cargarArchivo(archivoHeader& header,std::fstream& archivo,int pos){
    templeatcito objeto;
    archivo.clear();
    archivo.seekg(pos*sizeof(templeatcito),std::ios::beg);
    archivo.read(reinterpret_cast<char*>(&objeto),sizeof(templeatcito));
    archivo.clear();
    return objeto;
}