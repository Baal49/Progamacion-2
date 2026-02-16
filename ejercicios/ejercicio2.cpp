/*
 * EJERCICIO CLASE 1 - PUNTEROS Y STRUCTS
 * ======================================
 *
 * ENUNCIADO SIMPLIFICADO:
 * Implementar un programa que permita:
 * 1. Crear una escuela con arreglos dinamicos de estudiantes y materias
 * 2. Llenar estudiantes y materias con datos del usuario
 * 3. Mostrar la escuela en formato de tabla
 * 4. Encontrar el estudiante con mejor promedio
 * 5. Calcular el promedio general de la escuela
 * 6. Buscar un estudiante por su id
 * 7. Liberar la memoria correctamente
 *
 * REQUISITOS OBLIGATORIOS:
 * - Usar punteros y memoria dinamica (new/delete[])
 * - Usar structs para representar escuela, estudiantes y materias
 * - La escuela debe tener punteros a estudiantes y materias
 * - Validar entrada del usuario (cantidad > 0)
 * - Liberar toda la memoria asignada y asignar nullptr
 * - Usar paso por referencia cuando sea apropiado
 *
 * FUNCIONES QUE DEBE IMPLEMENTAR:
 * 1. void crearEscuela(Escuela& escuela, int cantEst, int cantMat)
 * 2. void llenarEstudiantes(Escuela& escuela)
 * 3. void llenarMaterias(Escuela& escuela)
 * 4. void mostrarEscuela(const Escuela& escuela)
 * 5. int buscarMejorPromedio(const Escuela& escuela)
 * 6. float calcularPromedioGeneral(const Escuela& escuela)
 * 7. int buscarEstudiantePorId(const Escuela& escuela, int id)
 * 8. void liberarEscuela(Escuela& escuela)
 *
 * MENU SIMPLE:
 * 1. Crear y llenar escuela
 * 2. Mostrar escuela
 * 3. Estudiante con mejor promedio
 * 4. Promedio general
 * 5. Buscar por id
 * 6. Salir
 *
 * NOTAS IMPORTANTES:
 * - Validar que la cantidad sea positiva
 * - Verificar punteros antes de usar
 * - Manejar el caso de lista vacia
 * - Usar buenas practicas de programacion
 *
 * CRITERIOS DE EVALUACION:
 * - Correcta implementacion de punteros (40%)
 * - Uso de structs y manejo de memoria (40%)
 * - Funcionalidad y validaciones (20%)
 *
 * TIEMPO ESTIMADO: 1-2 horas
 * DIFICULTAD: Media
 */

#include <iostream>
#include <iomanip>
#include <limits>
using namespace std;

struct Materia {
    int codigo;
    char nombre[40];
    int creditos;
};

struct Estudiante {
    int id;
    char nombre[40];
    float promedio;
    int codigoMateriaPrincipal;
};

struct Escuela {
    Estudiante* estudiantes;
    int cantidadEstudiantes;
    Materia* materias;
    int cantidadMaterias;
};

// Crea la escuela con arreglos dinamicos
void crearEscuela(Escuela& escuela, int cantEst, int cantMat) {
    if (cantEst <= 0 || cantMat <= 0) {
        cout << "Error: La cantidad de estudiantes y materias debe ser mayor a 0." << endl;
        escuela.estudiantes = nullptr;
        escuela.materias = nullptr;
        escuela.cantidadEstudiantes = 0;
        escuela.cantidadMaterias = 0;
        return;
        }
    else 
    escuela.estudiantes = new Estudiante[cantEst];
    escuela.materias = new Materia[cantMat];
    escuela.cantidadEstudiantes = cantEst;
    escuela.cantidadMaterias = cantMat;
    if (escuela.estudiantes == nullptr || escuela.materias == nullptr) {
        cout << "Error al asignar memoria para la escuela." << endl;
        escuela.estudiantes = nullptr;
        escuela.materias = nullptr;
        escuela.cantidadEstudiantes = 0;
        escuela.cantidadMaterias = 0;
        delete[] escuela.estudiantes;
        delete[] escuela.materias;
    }
    // TODO: Implementar esta funcion
    // - Verificar que escuela.estudiantes no sea nullptr
    // - Pedir id, nombre y promedio
    // - Pedir codigoMateriaPrincipal (debe existir en materias)
    // - Usar cin.getline para el nombre
}

// Llena la lista de materias
void llenarMaterias(Escuela& escuela) {
    if (escuela.materias == nullptr) {
        cout << "Error: No hay espacio para materias. Cree la escuela primero." << endl;
        return;
    }
        for (int i = 0; i < escuela.cantidadMaterias; i++) {
            cout << "Materia " << (i + 1) << ":" << endl;
            cout << "Codigo: ";
            cin >> escuela.materias[i].codigo;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar buffer
            cout << "Nombre: ";
            cin.getline(escuela.materias[i].nombre, 40);
            cout << "Creditos: ";
            cin >> escuela.materias[i].creditos;
        }
    // TODO: Implementar esta funcion
    // - Verificar que escuela.materias no sea nullptr
    // - Pedir codigo, nombre y creditos
}

void llenarEstudiantes(Escuela& escuela, int index = 0) {
    if (escuela.estudiantes == nullptr) {
        cout << "Error: No hay espacio para estudiantes. Cree la escuela primero." << endl;
        return;
    }
    else 
    cout << "Estudiante " << (index + 1) << ":" << endl;
    cout << "ID: ";
    cin >> escuela.estudiantes[index].id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar buffer
    cout << "Nombre: ";
    cin.getline(escuela.estudiantes[index].nombre, 40);
    cout << "Promedio: ";
    cin >> escuela.estudiantes[index].promedio;
    cout << "Codigo de materia principal: ";
    cin >> escuela.estudiantes[index].codigoMateriaPrincipal;
}

// Muestra la escuela completa
void mostrarEscuela(const Escuela& escuela) {
    if (escuela.materias == nullptr || escuela.estudiantes == nullptr) {
        cout << "No hay escuela creada o esta vacia." << endl;
        return;
    }
    cout << "\n=== MATERIAS ===" << endl;
    cout << left << setw(10) << "Codigo" << setw(40) << "Nombre" << setw(10) << "Creditos" << endl;
    for (int i = 0; i < escuela.cantidadMaterias; i++) {
        cout << left << setw(10) << escuela.materias[i].codigo
             << setw(40) << escuela.materias[i].nombre
             << setw(10) << escuela.materias[i].creditos << endl;
    }
    // TODO: Implementar esta funcion
    // - Verificar que los punteros no sean nullptr
    // - Mostrar materias primero y luego estudiantes
    // - Usar tabla simple con setw
}

// Retorna el indice del estudiante con mejor promedio
int buscarMejorPromedio(const Escuela& escuela) {
    if (escuela.estudiantes == nullptr || escuela.cantidadEstudiantes == 0) {
        return -1; // No hay estudiantes
    }
    int indiceMax = 0;
    for (int i = 1; i < escuela.cantidadEstudiantes; i++) {
        if (escuela.estudiantes[i].promedio > escuela.estudiantes[indiceMax].promedio) {
            indiceMax = i;
        }
    }
    // TODO: Implementar esta funcion
    // - Verificar que escuela.estudiantes no sea nullptr y cantidadEstudiantes > 0
    // - Comparar promedios para encontrar el maximo
    // - Retornar el indice encontrado
    return -1; // Placeholder
}

// Calcula el promedio general de la escuela
float calcularPromedioGeneral(const Escuela& escuela) {
    if (escuela.estudiantes == nullptr || escuela.cantidadEstudiantes == 0) {
        return 0.0f; // No hay estudiantes
    }
    float suma = 0.0f;
    for (int i = 0; i < escuela.cantidadEstudiantes; i++) {
        suma += escuela.estudiantes[i].promedio;
    }
    // TODO: Implementar esta funcion
    // - Verificar que escuela.estudiantes no sea nullptr y cantidadEstudiantes > 0
    // - Sumar promedios y dividir por cantidadEstudiantes
    return 0.0f; // Placeholder
}

// Busca un estudiante por id y retorna su indice
int buscarEstudiantePorId(const Escuela& escuela, int id) {
    if (escuela.estudiantes == nullptr || escuela.cantidadEstudiantes == 0) {
        return -1; // No hay estudiantes
    }
    for (int i = 0; i < escuela.cantidadEstudiantes; i++) {
        if (escuela.estudiantes[i].id == id) {
            return i; // Encontrado
        }
    }
    // TODO: Implementar esta funcion
    // - Verificar que escuela.estudiantes no sea nullptr
    // - Recorrer estudiantes y comparar id
    // - Retornar indice o -1 si no existe
    return -1; // Placeholder
}

// Libera la memoria de la escuela
void liberarEscuela(Escuela& escuela) {
    delete [] escuela.estudiantes;
    delete [] escuela.materias;
    escuela.estudiantes = nullptr;
    escuela.materias = nullptr;
    escuela.cantidadEstudiantes = 0;
    escuela.cantidadMaterias = 0;
    // TODO: Implementar esta funcion
    // - Liberar estudiantes y materias con delete[]
    // - Asignar nullptr a ambos punteros
    // - Poner cantidades en 0
}

// Muestra el menu principal
void mostrarMenu() {
    cout << "\n=== GESTION DE ESTUDIANTES ===" << endl;
    cout << "1. Crear y llenar escuela" << endl;
    cout << "2. Mostrar escuela" << endl;
    cout << "3. Estudiante con mejor promedio" << endl;
    cout << "4. Promedio general" << endl;
    cout << "5. Buscar por id" << endl;
    cout << "6. Salir" << endl;
    cout << "Seleccione una opcion: ";
}

int main() {
    Escuela escuela;
    escuela.estudiantes = nullptr;
    escuela.materias = nullptr;
    escuela.cantidadEstudiantes = 0;
    escuela.cantidadMaterias = 0;

    cout << "=== GESTION DE ESTUDIANTES ===" << endl;
    cout << "Implemente las funciones marcadas con TODO para completar el ejercicio." << endl;

    int opcion;
    do {
        mostrarMenu();
        cin >> opcion;

        switch (opcion) {
            case 1: {
                int cantEst = 0;
                int cantMat = 0;
                cout << "Ingrese la cantidad de estudiantes: ";
                cin >> cantEst;
                cout << "Ingrese la cantidad de materias: ";
                cin >> cantMat;
                cout << "Creando escuela con " << cantEst << " estudiantes y " << cantMat << " materias..." << endl;

                liberarEscuela(escuela);
                crearEscuela(escuela, cantEst, cantMat);

                if (escuela.estudiantes != nullptr && escuela.materias != nullptr) {
                    llenarMaterias(escuela);
                    llenarEstudiantes(escuela);
                    cout << "Escuela creada y llenada correctamente." << endl;
                } else {
                    cout << "Error al crear la escuela." << endl;
                }
                break;
            }

            case 2: {
                if (escuela.estudiantes != nullptr && escuela.materias != nullptr) {
                    mostrarEscuela(escuela);
                } else {
                    cout << "No hay escuela creada. Use la opcion 1 primero." << endl;
                }
                break;
            }

            case 3: {
                if (escuela.estudiantes != nullptr && escuela.cantidadEstudiantes > 0) {
                    int indice = buscarMejorPromedio(escuela);
                    if (indice >= 0) {
                        cout << "Mejor promedio: " << escuela.estudiantes[indice].nombre
                             << " (" << fixed << setprecision(2)
                             << escuela.estudiantes[indice].promedio << ")" << endl;
                    } else {
                        cout << "No hay estudiantes en la lista." << endl;
                    }
                } else {
                    cout << "No hay escuela creada o esta vacia." << endl;
                }
                break;
            }

            case 4: {
                if (escuela.estudiantes != nullptr && escuela.cantidadEstudiantes > 0) {
                    float promedio = calcularPromedioGeneral(escuela);
                    cout << "Promedio general: " << fixed << setprecision(2)
                         << promedio << endl;
                } else {
                    cout << "No hay escuela creada o esta vacia." << endl;
                }
                break;
            }

            case 5: {
                if (escuela.estudiantes != nullptr && escuela.cantidadEstudiantes > 0) {
                    int idBuscado;
                    cout << "Ingrese el id a buscar: ";
                    cin >> idBuscado;
                    int indice = buscarEstudiantePorId(escuela, idBuscado);
                    if (indice >= 0) {
                        cout << "Encontrado: " << escuela.estudiantes[indice].nombre
                             << " (" << fixed << setprecision(2)
                             << escuela.estudiantes[indice].promedio << ")" << endl;
                    } else {
                        cout << "No se encontro un estudiante con ese id." << endl;
                    }
                } else {
                    cout << "No hay escuela creada o esta vacia." << endl;
                }
                break;
            }

            case 6: {
                cout << "Saliendo del programa..." << endl;
                break;
            }

            default: {
                cout << "Opcion invalida. Intente nuevamente." << endl;
                break;
            }
        }
    } while (opcion != 6);

    liberarEscuela(escuela);
    cout << "Programa finalizado. Memoria liberada." << endl;
    return 0;
}

/*
 * PISTAS PARA LA IMPLEMENTACION:
 *
 * 1. crearEscuela(Escuela& escuela, int cantEst, int cantMat):
 *    - Verificar que cantEst y cantMat > 0
 *    - Crear arreglos con new
 *    - Asignar punteros y cantidades
 *
 * 2. llenarMaterias(Escuela& escuela):
 *    - Usar cin.ignore(numeric_limits<streamsize>::max(), '\n');
 *    - Leer nombre con cin.getline(escuela.materias[i].nombre, 40)
 *
 * 3. llenarEstudiantes(Escuela& escuela):
 *    - Pedir id, nombre, promedio y codigoMateriaPrincipal
 *
 * 4. mostrarEscuela(const Escuela& escuela):
 *    - Mostrar materias primero y luego estudiantes
 *
 * 5. buscarMejorPromedio(const Escuela& escuela):
 *    - Inicializar indiceMax = 0
 *    - Comparar estudiantes[i].promedio
 *
 * 6. calcularPromedioGeneral(const Escuela& escuela):
 *    - Acumular suma y dividir por cantidadEstudiantes
 *
 * 7. buscarEstudiantePorId(const Escuela& escuela, int id):
 *    - Recorrer estudiantes y comparar id
 *
 * 8. liberarEscuela(Escuela& escuela):
 *    - delete[] estudiantes; delete[] materias
 *    - Asignar nullptr a ambos y poner cantidades en 0
 */