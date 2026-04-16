#ifndef HEADER_HPP
#define HEADER_HPP

#include <fstream>
#include <string>

struct archivoHeader{
    int cantidadRegistros;      // Total histórico de registros
    int proximoID;              // Siguiente ID a asignar (Autoincremental)
    int registrosActivos;       // Registros que no están marcados como eliminados
    int version;     
    void actualizarheader(archivoHeader header,std::fstream& archivo);       // Control de versión del archivo
};
#endif // HEADER_HPP