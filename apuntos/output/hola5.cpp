#include <iostream>
#include <string>
#include <random>

using namespace std;

int main(){

    cout << "Bienvenido al bucle de Fibonacci!" << endl;
    cout << "Un hechicero te pide generar los primeros numeros de la serie de fibonacci, pero con un giro, nosotros te pediremos un numero a ti" << endl;

    int n;
    cout << "Cuantos numeros de la serie de Fibonacci quieres generar? ";
    cin >> n;

    while (n < 0) {
        cout << "Por favor ingresa un numero no negativo: ";
        cin >> n;
    }

    if (n == 0) {
        cout << "No hay numeros para generar." << endl;
    } else {
        int a = 0, b = 1;
        cout << "Serie de Fibonacci:" << endl;
        for (int i = 0; i < n; i++) {
            if (a % 2 == 0) {
                cout << "\033[31m*" << a << "*\033[0m ";
            } else {
                cout << a << " ";
            }
            int temp = a + b;
            a = b;
            b = temp;
        }
        cout << endl;
    }

    return 0;
}

