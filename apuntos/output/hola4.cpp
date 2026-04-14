#include <iostream>
#include <string>
#include <random>

using namespace std;

int determinarResultado(int usuario, int computadora) {
    if (usuario == computadora) return 0;
    switch (usuario) {
        case 1: // piedra
            switch (computadora) {
                case 3: // tijera
                case 4: // lagarto
                    return 1; // gana usuario
                default:
                    return 2; // pierde
            }
        case 2: // papel
            switch (computadora) {
                case 1: // piedra
                case 5: // spock
                    return 1;
                default:
                    return 2;
            }
        case 3: // tijera
            switch (computadora) {
                case 2: // papel
                case 4: // lagarto
                    return 1;
                default:
                    return 2;
            }
        case 4: // lagarto
            switch (computadora) {
                case 2: // papel
                case 5: // spock
                    return 1;
                default:
                    return 2;
            }
        case 5: // spock
            switch (computadora) {
                case 1: // piedra
                case 3: // tijera
                    return 1;
                default:
                    return 2;
            }
    }
    return -1; // error
}

int main() {
    int opciones;
    int piedra = 1;
    int papel = 2;
    int tijera = 3;
    int lagarto = 4;
    int spock = 5;
    int computadora;
    int usuario;
    int victorias_usuario = 0;
    int victorias_computadora = 0;

    cout << "Bienvenido al juego de Piedra, Papel o Tijera o Lagarto o Spock!" << endl;
    cout << "Elige tu opcion: " << endl;
    cout << "Reglas rapidas:" << endl;
    cout << "Piedra aplasta Tijera y Lagarto" << endl;
    cout << "Papel cubre Piedra y aplasta Lagarto" << endl;
    cout << "Tijera corta Papel y mata Lagarto" << endl;
    cout << "Lagarto envenena Spock y come Papel" << endl;
    cout << "Spock destruye Tijera y vaporiza Piedra" << endl;

    cout << "1. Piedra" << endl;
    cout << "2. Papel" << endl;
    cout << "3. Tijera" << endl;
    cout << "4. Lagarto" << endl;
    cout << "5. Spock" << endl;
    cout << "Ingresa tu opcion (1-5): ";
    cin >> opciones;

    while (opciones < 1 || opciones > 5) {
        cout << "Opcion invalida. Por favor ingresa un numero entre 1 y 5: ";
        cin >> opciones;
    }
    while (victorias_usuario < 5 && victorias_computadora < 5) {
        computadora = rand() % 5 + 1; // Genera un numero aleatorio entre 1 y 5
        cout << "Ingresa tu opcion (1-5): ";
        cin >> usuario;

        while (usuario < 1 || usuario > 5) {
            cout << "Opcion invalida. Por favor ingresa un numero entre 1 y 5: ";
            cin >> usuario;
        }

        int resultado = determinarResultado(usuario, computadora);
        switch (resultado) {
            case 0:
                cout << "Empate!" << endl;
                break;
            case 1:
                cout << "Ganaste esta ronda!" << endl;
                victorias_usuario++;
                break;
            case 2:
                cout << "Perdiste esta ronda!" << endl;
                victorias_computadora++;
                break;
        }

        cout << "Victorias del usuario: " << victorias_usuario << endl;
        cout << "Victorias de la computadora: " << victorias_computadora << endl;
    }

    if (victorias_usuario == 5) {
        cout << "Felicidades! Ganaste el juego!" << endl;
    } else {
        cout << "La computadora gano el juego!" << endl;
    }

    return 0;
}