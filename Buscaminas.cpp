#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;

int** CreacionTablero (int dificultad){
    int** tablero;
    switch  (dificultad){
        case 1:
            tablero = new int*[10];
            for(int i=0; i < 10; i++){
                tablero[i]=new int[10];
            }
            break;
        case 2:
            tablero = new int*[15];
            for(int i=0; i < 15; i++){
                tablero[i]=new int[15];
            }
            break;
        case 3:
            tablero = new int*[20];
            for(int i=0; i < 20; i++){
                tablero[i]=new int[20];
            }
            break;
    }
    return tablero;
}

void GenerarMinas (int** tablero, int dificultad){
    srand(time(0));
    int TotalMinas;
    int FilaAleatoria;
    int ColumnaAleatoria;
    int MinasPuestas=0;
    switch (dificultad){
        case 1:
            TotalMinas=10;
            while(MinasPuestas<TotalMinas){
                FilaAleatoria = rand() % 10;
                ColumnaAleatoria = rand() % 10;
                if(tablero[FilaAleatoria][ColumnaAleatoria]!=10){
                    tablero[FilaAleatoria][ColumnaAleatoria]=10;
                    MinasPuestas++;    
                }
            }
            break;
        case 2:
            TotalMinas=30;
            while(MinasPuestas<TotalMinas){
                FilaAleatoria = rand() % 15;
                ColumnaAleatoria = rand() % 15;
                if(tablero[FilaAleatoria][ColumnaAleatoria]!=10){
                    tablero[FilaAleatoria][ColumnaAleatoria]=10;
                    MinasPuestas++;    
                }
            } 
            break;
        case 3:
            TotalMinas=60;
            while(MinasPuestas<TotalMinas){
                FilaAleatoria = rand() % 20;
                ColumnaAleatoria = rand() % 20;
                if(tablero[FilaAleatoria][ColumnaAleatoria]!=10){
                    tablero[FilaAleatoria][ColumnaAleatoria]=10;
                    MinasPuestas++;    
                }
            }   
            break;
    }
}

int contarMinasAlrededor(int** tablero, int fila, int col, int tam) {
    int contador = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int nx = fila + i;
            int ny = col + j;
            if (nx >= 0 && nx < tam && ny >= 0 && ny < tam) {
                if (tablero[nx][ny] == 10) {
                    contador++;
                }
            }
        }
    }
    return contador;
}

void RellenarNumeros(int** tablero, int dificultad) {
    int tam;
    switch(dificultad) {
        case 1: tam = 10; break;
        case 2: tam = 15; break;
        case 3: tam = 20; break;
        default: tam = 10; break;
    }

    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            if (tablero[i][j] != 10) {
                tablero[i][j] = contarMinasAlrededor(tablero, i, j, tam);
            }
        }
    }
}

void ImprimirTablero(int** tablero, int dificultad) {
    int tam;
    switch(dificultad) {
        case 1: tam = 10; break;
        case 2: tam = 15; break;
        case 3: tam = 20; break;
        default: tam = 10; break;
    }

    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            if (tablero[i][j] == 10) {
                cout << "* ";   // mina
            } else {
                cout << tablero[i][j] << " "; // número
            }
        }
        cout << endl;
    }
}

int main(){
    int dificultad;
    cout << "Ingrese dificultad (1=facil, 2=medio, 3=dificil): ";
    cin >> dificultad;
    int** tablero = CreacionTablero(dificultad);
    GenerarMinas(tablero, dificultad);
    RellenarNumeros(tablero, dificultad);
    ImprimirTablero(tablero,dificultad);
}

