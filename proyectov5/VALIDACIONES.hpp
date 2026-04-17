#ifndef VALIDACIONES_HPP
#define VALIDACIONES_HPP
#include <iostream>
#include <string>
#include <vector>
#include <cctype> 
#include "CLIENTES.hpp"  // Para Cliente

// Funciones de validación reutilizables
bool esNumero(const std::string& s) {
    for (char ch : s) {
        if (!std::isdigit(ch)) return false;
    }
    return true;
}
bool esVacio(const std::string& s) {
    return s.empty();
}
bool esCancelar(const std::string& s) {
    return s == "CANCELAR" || s == "0";
}
template<typename T>
bool convertirAEntero(const std::string& s, T& resultado) {
    try {
        resultado = std::stoi(s);
        return true;
    } catch (...) {
        std::cout << "Entrada inválida. Ingrese un número." << std::endl;
        return false;
    }
}
int opcionSN(const std::string& s) {
    if (s == "S" || s == "s" || s == "SI" || s == "si") return 1;  // Sí
    if (s == "N" || s == "n" || s == "NO" || s == "no") return 0;  // No
    return -1;  // Inválido
}
template<typename T>
bool esMayorQueCero(T n) {
    return n > 0;
}
template<typename templatecito ,typename atributotemplatecito, typename x>
bool existe(atributotemplatecito templatecito::*atributo,archivoHeader& header,fstream& archivo,x valor){
    templatecito temp;
    for(int i=0;i<header.cantidadRegistros;i++){
        temp=cargarArchivo<templatecito>(header,archivo,i);
        if(temp.*atributo==valor){
            return true;
        }
    }
    return false;
}
bool validarCedula(const std::string& input, int& cedulaNueva,archivoHeader& cliente,fstream& archivoclientes) {
    if (esVacio(input)) {
        std::cout << "La cédula no puede estar vacía." << std::endl;
        return false;
    }
    if (!esNumero(input)) {
        std::cout << "La cédula solo puede contener números." << std::endl;
        return false;
    }
    cedulaNueva = std::stoi(input);
    for (int i=0;i<cliente.cantidadRegistros;i++) {
        Cliente c=cargarArchivo<Cliente>(cliente,archivoclientes,i);
        if (c.getcedula() == cedulaNueva) {
            std::cout << "La cédula ya existe. Ingrese otra." << std::endl;
            return false;
        }
    }
    return true;
}
bool obtenerInput(std::string& input, const std::string& mensaje) {
    std::cout << mensaje;
    if (!std::getline(std::cin, input)) return false;
    if (esCancelar(input)) {
        std::cout << "Operación cancelada." << std::endl;
        return false;
    }
    return true;
}
bool validarNombre(const std::string& input) {
    if (esVacio(input)) {
        std::cout << "El nombre no puede estar vacío." << std::endl;
        return false;
    }
    return true;
}
bool validarStock(int cantidad, int stock) {
    if (cantidad > stock) {
        std::cout << "No hay suficiente stock. Stock disponible: " << stock << std::endl;
        return false;
    }
    return true;
}

#endif

