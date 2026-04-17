#include "VALIDACIONES.hpp"
#include <iostream>
#include <string>
#include <cctype>  // Para isdigit
#include <vector>

// Función para validar si una string es numérica
bool esNumero(const std::string& s) {
    for (char ch : s) {
        if (!std::isdigit(ch)) return false;
    }
    return true;
}

// Función para validar si una string está vacía
bool esVacio(const std::string& s) {
    return s.empty();
}

// Función para validar si es cancelar
bool esCancelar(const std::string& s) {
    return s == "CANCELAR" || s == "0";
}

// Template para conversión segura a entero
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

// Función para opciones S/N
int opcionSN(const std::string& s) {
    if (s == "S" || s == "s" || s == "SI" || s == "si") return 1;  // Sí
    if (s == "N" || s == "n" || s == "NO" || s == "no") return 0;  // No
    return -1;  // Inválido
}

// Template para validar si es mayor que cero
template<typename T>
bool esMayorQueCero(T n) {
    return n > 0;
}

// Función para validar cédula (combina checks)
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

// Función para obtener input con validación de cancelar
bool obtenerInput(std::string& input, const std::string& mensaje) {
    std::cout << mensaje;
    if (!std::getline(std::cin, input)) return false;
    if (esCancelar(input)) {
        std::cout << "Operación cancelada." << std::endl;
        return false;
    }
    return true;
}

// Función para validar nombre (no vacío)
bool validarNombre(const std::string& input) {
    if (esVacio(input)) {
        std::cout << "El nombre no puede estar vacío." << std::endl;
        return false;
    }
    return true;
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
// Función para validar stock (cantidad <= stock)
bool validarStock(int cantidad, int stock) {
    if (cantidad > stock) {
        std::cout << "No hay suficiente stock. Stock disponible: " << stock << std::endl;
        return false;
    }
    return true;
}

// Especializaciones de templates
template bool convertirAEntero<int>(const std::string& s, int& resultado);
template bool esMayorQueCero<int>(int n);
template bool esMayorQueCero<double>(double n);