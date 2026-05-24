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
				tablero[FilaAleatoria][ColumnaAleatoria]=10;
				MinasPuestas++;	
			}
			break;
		case 3:
			TotalMinas=60;
			while(MinasPuestas<TotalMinas){
				FilaAleatoria = rand() % 20;
				ColumnaAleatoria = rand() % 20;
				tablero[FilaAleatoria][ColumnaAleatoria]=10;
				MinasPuestas++;	
			}
			break;
	}
	
}
void ImprimirTablero(int** tablero, int dificultad) {
    switch(dificultad) {
        case 1:
        	for (int i = 0; i < 10; i++) {           
        		for (int j = 0; j < 10; j++) { 
					if(tablero[i][j]!=10){
						tablero[i][j]=0;
					}   
            		cout<< tablero[i][j] <<"|"; 
        		}
        		cout << endl; 
    		}
			break;
    }

}

int main(){
	int dificultad;
	cin>>dificultad;
	int** tablero = CreacionTablero(dificultad);
	GenerarMinas(tablero, dificultad);
	ImprimirTablero(tablero,dificultad);
	
}

