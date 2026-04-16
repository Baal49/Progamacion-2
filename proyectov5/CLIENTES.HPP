#ifndef CLIENTES_HPP
#define CLIENTES_HPP
#include <fstream>
#include "ARCHIVO.hpp"
#include "HEADER.hpp"
using namespace std;
class Cliente{
public:
 int cedula;
 char nombre[100];
 char correo[100];
 char telefono[20];
 char dirreccion[200];
 char fecharegitro[11]; //formato:YYYY-MM-DD
 Cliente buscarCliente(archivoHeader& cliente,fstream& archivoc,int cedula,string &nombre,string &direccion);
void Crearcliente(archivoHeader& cliente,fstream& archivoc);
        void Editarcliente (archivoHeader& cliente, fstream& archivoc, int cedula);
    void listarcliente(archivoHeader cliente,fstream* archivoc,int posi);
};
#endif // CLIENTES_HPP