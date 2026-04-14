#include <iostream>
#include <string>
using namespace std;

    int main () {

    cout << "Bienvenido al Dado Trucado o como se llame idk" << endl;
    cout << "Tiene 100 monedas, elija la cantidad de monedas a apostar:" << endl;
    int apuesta;
    cin >> apuesta;
    while (apuesta > 100 || apuesta <= 0) {
        cout << "No tienes suficientes monedas para apostar esa cantidad." << endl;
        cout << "Por favor, elija una cantidad válida:" << endl;
        cin >> apuesta;
    }
    
    cout << "Se lanzo un dado, elija si es un numero par o impar" << endl;
    string eleccion;
    cin >> eleccion;
    
    while (eleccion != "par" && eleccion != "impar") {
        cout << "Eleccion invalida. Por favor, elija 'par' o 'impar':" << endl;
        cin >> eleccion;
    }
    int resultado = rand() % 6 + 1; // Genera un numero entre 1 y 6
    cout << "El resultado del dado es: " << resultado << endl;
    bool esPar = (resultado % 2 == 0);
    if ((esPar && eleccion == "par") || (!esPar && eleccion == "impar")) {
        cout << "¡Felicidades! Has adivinado correctamente." << endl;
        cout << "Has ganado " << apuesta*5 << " monedas." << endl;
        cout << "Desea intentarlo de nuevo?" << endl;
        string respuesta;
        cin >> respuesta;
        if (respuesta == "si") {
            main(); // Reinicia el juego
        } else {
            cout << "Gracias por jugar. ¡Me piro vampiro!" << endl;
        }

    } else {
        cout << "Incorrecto. Has perdido " << apuesta << " monedas." << endl;
        cout << "Desea intentarlo de nuevo?" << endl;
        string respuesta;
        cin >> respuesta;
        if (respuesta == "si") {
            main(); // Reinicia el juego
        } else {
            cout << "Gracias por jugar. ¡Me piro vampiro!" << endl;
        }
    }
    return 0;
    }