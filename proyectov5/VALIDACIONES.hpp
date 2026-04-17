#ifndef VALIDACIONES_HPP
#define VALIDACIONES_HPP

#include <string>
#include <vector>
#include "CLIENTES.hpp"  // Para Cliente

// Funciones de validación reutilizables
bool esNumero(const std::string& s);
bool esVacio(const std::string& s);
bool esCancelar(const std::string& s);
template<typename T> bool convertirAEntero(const std::string& s, T& resultado);
int opcionSN(const std::string& s);
template<typename T> bool esMayorQueCero(T n);
template<typename templatecito ,typename atributotemplatecito, typename x>
bool existe(atributotemplatecito templatecito::*atributo,archivoHeader& header,fstream& archivo,x valor);
bool validarCedula(const std::string& input, int& cedulaNueva, const std::vector<Cliente>& listaClientes);
bool obtenerInput(std::string& input, const std::string& mensaje);
bool validarNombre(const std::string& input);
bool validarStock(int cantidad, int stock);

#endif

