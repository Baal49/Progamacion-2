#ifndef ARCHIVO_HPP
#define ARCHIVO_HPP
#include <fstream>
#include "HEADER.hpp"
template<typename ola>
void guardarArchivo(archivoHeader& header,std::fstream& archivo,ola& objeto,int pos=-1);
template<typename ola>
ola cargarArchivo(archivoHeader& header,std::fstream& archivo,int pos);
#endif // ARCHIVO_HPP