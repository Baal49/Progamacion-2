#ifndef TRANSACCION_hpp
#define TRANSACCION_hpp
#include <fstream>
#include "HEADER.hpp"
#include "PROVEEDOR.hpp"
#include "PRODUCTO.hpp"
#include "PRODUCTOVENTA.hpp"
#include "ARCHIVO.hpp"
class Transaccion {
    public:
    int idt=0;
    int idcp=0;
    int tipo;
    float total;
    char fecha[11];
    char descripcion[200];//opcional

    void venta(archivoHeader& producto,archivoHeader transaccion,archivoHeader& cliente,archivoHeader& pt,fstream& archivop,fstream& archivoc,fstream& archivot,fstream& archivopt);
void compra(archivoHeader producto, archivoHeader proveedor,archivoHeader cliente,archivoHeader& transaccion,archivoHeader& pt,fstream& archivop,fstream& archivoprov,fstream& archivoc,fstream& archivot,fstream& archivopt);
void buscarTransaccionesPorProducto(archivoHeader& transaccion,fstream& archivot,int idProducto);
 void Buscartransaccion (archivoHeader&transaccion,archivoHeader pt,fstream& archivot,fstream& archivopt,int id);
void listartransaccion(archivoHeader transaccion,archivoHeader pt,fstream& archivot,fstream& archivopt,int posi);
};
#endif // TRANSACCION_hpp