#ifndef HEADER_HPP
#define HEADER_HPP
struct archivoHeader{
    int cantidadRegistros;      // Total histórico de registros
    int proximoID;              // Siguiente ID a asignar (Autoincremental)
    int registrosActivos;       // Registros que no están marcados como eliminados
    int version;                // Control de versión del archivo
};
#endif // HEADER_HPP