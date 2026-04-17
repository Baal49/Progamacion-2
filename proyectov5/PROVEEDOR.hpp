#ifndef PROVEEDOR_HPP
#define PROVEEDOR_HPP
#include "ARCHIVO.hpp"
#include <string>
#include <fstream>
#include "HEADER.hpp"
#include <limits>
#include <cstring>
using namespace std;
    class Proveedor {
    private:
    int id=0;
    char nombre[100];
    bool activo=1;
    public:
    int getid();
    const char* getnombre();
    bool getact();
    void Crearproveedor(archivoHeader& proveedor, fstream* archivo,int posi);
    void listarProveedores(archivoHeader proveedor,fstream* archivoprov,int posi);

    void eliminarProveedor(archivoHeader proveedores,fstream& archivop,int id);

    void editarProveedor(archivoHeader proveedor,fstream &archivop, int idProveedor);

    Proveedor* buscarProveedor(archivoHeader proveedor,fstream* archivo,int id,string nombre,int opcion);
    };
    #endif // PROVEEDOR_HPP