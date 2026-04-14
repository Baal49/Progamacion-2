#include <iostream>
#include <string>
#include <random>
#include <cstdlib>
#include <ctime>

using namespace std;

string Pokemonrandom(int num) {
    switch(num) {
        case 1: return "Pikachu";
        case 2: return "Charmander";
        case 3: return "Squirtle";
        case 4: return "Bulbasaur";
        case 5: return "Jigglypuff";
        default: return "Desconocido";
    }
}

string getHint(int num, int index) {
    switch(num) {
        case 1: // Pikachu
            if(index == 0) return "Es amarillo.";
            if(index == 1) return "Es de tipo electrico.";
            break;
        case 2: // Charmander
            if(index == 0) return "Parece un lagarto.";
            if(index == 1) return "Es de tipo fuego.";
            break;
        case 3: // Squirtle
            if(index == 0) return "Parece una tortuga.";
            if(index == 1) return "Es de tipo agua.";
            break;
        case 4: // Bulbasaur
            if(index == 0) return "Tiene un bulbo en la espalda.";
            if(index == 1) return "Es de tipo planta.";
            break;
        case 5: // Jigglypuff
            if(index == 0) return "Es rosa.";
            if(index == 1) return "Es de tipo normal.";
            break;
        default: return "No hay pista disponible.";
    }
    return "No hay mas pistas.";
}

int pokemon() {
    return rand() % 5 + 1;
}

int main () {
    srand(time(NULL));
    int randomPokemon = pokemon();
    cout << "Bienvenido a Minijuegos.com (por favor entiendan la referencia)" << endl;
    cout << "El sistema ha elegido un Pokemon al azar. Tienes 3 intentos para adivinar cual es (1-5):" << endl;
    cout << "1: Pikachu" << endl;
    cout << "2: Charmander" << endl;
    cout << "3: Squirtle" << endl;
    cout << "4: Bulbasaur" << endl;
    cout << "5: Jigglypuff" << endl;

    int hintIndex = 0;
    for(int attempt = 1; attempt <= 3; attempt++) {
        cout << "Intento " << attempt << ": Ingresa tu adivinanza (1-5): ";
        int guess;
        cin >> guess;
        if(guess < 1 || guess > 5) {
            cout << "Numero invalido. Debe ser entre 1 y 5." << endl;
            attempt--; // no cuenta como intento
            continue;
        }
        if(guess == randomPokemon) {
            cout << "¡Felicidades! Has adivinado correctamente. El Pokemon era " << Pokemonrandom(randomPokemon) << "." << endl;
            return 0;
        } else {
            cout << "Incorrecto.";
            if(hintIndex < 2) {
                cout << " Pista " << hintIndex+1 << ": " << getHint(randomPokemon, hintIndex) << endl;
                hintIndex++;
            } else {
                cout << endl;
            }
            if(attempt < 3) {
                cout << "Intentos restantes: " << 3 - attempt << endl;
            }
        }
    }
    cout << "Se acabaron los intentos. El Pokemon era " << Pokemonrandom(randomPokemon) << "." << endl;
    return 0;
}