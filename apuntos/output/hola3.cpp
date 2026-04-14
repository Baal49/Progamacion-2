#include <iostream>
#include <string>

using namespace std;

int main() {
    int intentos = 0;
    int digito1 = 4; // 3 numeros que el usuario tiene que adivinar, esta forzado pero igual
    int digito2 = 2; 
    int digito3 = 9;
    string name;
    cout << "Bienvenido a la caja fuerte del Tesoro!" << endl;
    cout << "Tienes infinitos intentos para adivinar la clave de 3 digitos" << endl;
    cout << "Cada digito esta entre el 0 y el 9" << endl;

    while (true) {
        int intento1, intento2, intento3;
        cout << "Ingresa tu intento (3 digitos separados por espacios): ";
        cin >> intento1 >> intento2 >> intento3;
        intentos++;

        if (intento1 == digito1 && intento2 == digito2 && intento3 == digito3) {
            cout << "Felicidades! Has adivinado la clave en " << intentos << " intentos!" << endl;
            break;
        } else {
            cout << "Clave incorrecta." << endl;

            if (intento1 < digito1) {
                cout << "- El primer digito es muy bajo." << endl;
            } else if (intento1 > digito1) {
                cout << "- El primer digito es muy alto." << endl;
            } else {
                cout << "- El primer digito es correcto." << endl;
            }

            if (intento2 < digito2) {
                cout << "- El segundo digito es muy bajo." << endl;
            } else if (intento2 > digito2) {
                cout << "- El segundo digito es muy alto." << endl;
            } else {
                cout << "- El segundo digito es correcto." << endl;
            }

            if (intento3 < digito3) {
                cout << "- El tercer digito es muy bajo." << endl;
            } else if (intento3 > digito3) {
                cout << "- El tercer digito es muy alto." << endl;
            } else {
                cout << "- El tercer digito es correcto." << endl;
            }
        }
    }
    return 0;
}
