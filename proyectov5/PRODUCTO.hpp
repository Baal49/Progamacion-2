#ifndef PRODUCTO_HPP
#define PRODUCTO_HPP
#include <fstream>
#include <iostream>
#include <string>
#include "HEADER.hpp"
#include "PROVEEDOR.hpp"
  class Producto {
    public:
    int id=0;                    // Identificador único (autoincremental)
    char codigo[20];           // Código del producto (ej: "PROD-001")
    char nombre[100];          // Nombre del producto
    char descripcion[200];     // Descripción del producto
    int idProveedor=0;           // ID del proveedor asociado
    float precio=0;            // Precio unitario
    int stock=0;               // Cantidad en inventario
    char fechaRegistro[11];    // Formato: YYYY-MM-DD
    char fechavencimiento[11]; //Formato: YYYY-MM-DD (opcional)
    bool registroactivo;
    void Crearproductos(archivoHeader* proveedores,fstream& archivoproducto,archivoHeader& tienda,fstream& archivoproveedor,int posiprod,int posiprov);
    Producto buscarProducto(archivoHeader* productosheader,fstream& archivo,int id,string nombre, int opcion);
bool codigoDuplicado(archivoHeader* tienda,fstream* archivo ,const string& codigo);
void listarProductos(fstream* archivop,fstream* archivosprov, archivoHeader productos, archivoHeader provedor,int posiprod,int posiprov);
void eliminarProducto(archivoHeader &productos,fstream& archivop,int id);
void modificarstock(archivoHeader productos,fstream& archivop,int id,int cant,int opt);
void editarProducto(archivoHeader &producto,fstream &archivo,fstream* archivoprov, int idProducto);
};
#endif // PRODUCTO_HPP