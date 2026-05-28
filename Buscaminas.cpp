#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cctype>

using namespace std;

int** CreacionTablero (int dificultad){
    int** tablero;
    int tamano;
    switch(dificultad) {
        case 1: tamano = 10; break;
        case 2: tamano = 15; break;
        case 3: tamano = 20; break;
        default: tamano = 10; break;
    }
    tablero=new int*[tamano];
    for(int i=0; i < tamano; i++){
        tablero[i]=new int[tamano];
        for(int j=0;j<tamano;j++){
        	tablero[i][j]=0;
		}
    }          
    return tablero;
}
bool GenerarMinas (int** tablero, int dificultad, string coordenada){
    srand(time(0));
    int TotalMinas;
    int FilaAleatoria;
    int ColumnaAleatoria;
    int MinasPuestas=0;
    int tamano;
    stringstream ss(coordenada);
		string filaStr, columnaStr;
		int fila, columna;
		getline(ss,filaStr,',');
		getline(ss, columnaStr);
		fila=stoi(filaStr)-1;
		columna=stoi(columnaStr)-1;
    switch (dificultad){
        case 1: tamano=10;TotalMinas=10;break;
        case 2: tamano=15;TotalMinas=30;break;
        case 3: tamano=20;TotalMinas=60;break;
            TotalMinas=10;
    }
    if(fila>=0 && fila<tamano && columna>=0 && columna<tamano){
	    for (int i = -1; i <= 1; i++) {
	        for (int j = -1; j <= 1; j++) {
	            int nx = fila + i;
	            int ny = columna + j;
	            if (nx >= 0 && nx < tamano && ny >= 0 && ny < tamano) {
	                if (tablero[nx][ny] == 0) {
	                    tablero[nx][ny]=1;
	                }
	            }
	        }
	    }
	    while(MinasPuestas<TotalMinas){
	        FilaAleatoria = rand() % tamano;
	        ColumnaAleatoria = rand() % tamano;
	        if(tablero[FilaAleatoria][ColumnaAleatoria]!=10 && tablero[FilaAleatoria][ColumnaAleatoria]!=1){
	            tablero[FilaAleatoria][ColumnaAleatoria]=10;
	            MinasPuestas++;    
	        }
	    }
		return true;	
	}else{
		return false;
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
unsigned char** crearTableroVisual (int dificultad){
    unsigned char** tableroVisual;
    int tamano;
    switch  (dificultad){
        case 1:tamano=10;
        	break;
        case 2:tamano=15;
            break;
        case 3:tamano=20;
            break;
    }
        tableroVisual = new unsigned char*[tamano];
    	for(int i=0; i < tamano; i++){
            tableroVisual[i]=new unsigned char[tamano];
            for(int j=0;j<tamano;j++){
            	tableroVisual[i][j]=178;
			}
        }
    return tableroVisual;
}
bool verificarJugada(int dificultad, unsigned char** tableroVisual, int fila, int columna){
	int tamano;
	switch (dificultad){
		case 1:tamano=10;
			break;
		case 2: tamano=15;
			break;
		case 3: tamano=20;
			break;
	}
	if((fila>=0 && fila<tamano)&&(columna>=0 && columna<tamano)){
		if(tableroVisual[fila][columna]==178 || tableroVisual[fila][columna]==201){
			return 1;
		}else{
			return 0;
		}
	}else{
		return 0;
	}
}
void floodFill (int** tablero, unsigned char** tableroVisual, int fila, int columna, int dificultad){
	int tam;
    switch(dificultad) {
        case 1: tam = 10; break;
        case 2: tam = 15; break;
        case 3: tam = 20; break;
        default: tam = 10; break;
    }
	if(tablero[fila][columna]==0){
		tableroVisual[fila][columna]=tablero[fila][columna]+'0';
		for (int i = -1; i <= 1; i++) {
	        for (int j = -1; j <= 1; j++) {
	            int nx = fila + i;
	            int ny = columna + j;
	            if (nx >= 0 && nx < tam && ny >= 0 && ny < tam) {
	            	if(tableroVisual[nx][ny]==178){
	            		if(tablero[nx][ny]>0 && tablero[nx][ny]<=8){
	            			tableroVisual[nx][ny]=tablero[nx][ny]+'0';
						}else if(tablero[nx][ny]==0){	
							tableroVisual[nx][ny]=tablero[nx][ny]+'0';
	                		floodFill(tablero, tableroVisual, nx, ny, dificultad);
						}
					}
	            }
	        }
   		}
	}
}
bool imprimirTableroVisual(int dificultad, int** tablero, unsigned char** tableroVisual, int fila, int columna,string jugada){
	string linea;
	int banderas;
	int tamano;
	switch (dificultad){
		case 1:tamano=10;
			banderas=10;
			break;
		case 2: tamano=15;
			banderas=30;
			break;
		case 3: tamano=20;
			banderas=60;
			break;
	}
	if(jugada=="bandera"){
		tableroVisual[fila][columna]=201;
		banderas--;
	}else if(tablero[fila][columna]==10){
		tableroVisual[fila][columna]=42;
		return 0;
	}else if(tablero[fila][columna]==0){
		floodFill(tablero, tableroVisual, fila, columna, dificultad);
	}else{
		tableroVisual[fila][columna]=tablero[fila][columna]+'0';
	}
	for (int i = 0; i < tamano; i++) {
        for (int j = 0; j < tamano; j++) {
            cout<<tableroVisual[i][j]<<"  ";
        }
        cout <<endl<<endl;
    }
    cout<<"Banderas: "<<banderas<<endl;
}
void ImprimirTablero(int dificultad, unsigned char** tableroJugador, int** tablero, bool valor){
    int tam;
    switch(dificultad) {
        case 1: tam = 10; break;
        case 2: tam = 15; break;
        case 3: tam = 20; break;
        default: tam = 10; break;
    }
	if(valor){
		for(int i=0; i<tam; i++){
			for(int j=0;j<tam;j++){
				cout<<tableroJugador[i][j]<<"  ";
			}cout<<endl<<endl;
		}
	}else{
	    for (int i = 0; i < tam; i++) {
	        for (int j = 0; j < tam; j++) {
	        	if(tablero[i][j]==10){
	        		tableroJugador[i][j]=88;
				}
				if(tableroJugador[i][j]==178){
					tableroJugador[i][j]=tablero[i][j]+'0';
				}
	            cout<<tableroJugador[i][j]<<"  ";
	        }
	        cout <<endl<<endl;
	    }	
	}
}

int main(){
    int dificultad;
    string jugada; 
    cout << "Ingrese dificultad: "<<endl;
    cout<<"1. Facil, tablero 10x10, 10 minas"<<endl;
    cout<<"2. Medio, tablero 15x15, 30 minas"<<endl;
    cout<<"1. Facil, tablero 20x20, 60 minas"<<endl;
    cin >> dificultad;
    unsigned char** tableroVisual=crearTableroVisual(dificultad);
    bool valor=1;
    int** tablero;
    ImprimirTablero(dificultad, tableroVisual, tablero, valor);
    cout<<"Ingrese jugada: ";
    do{
		cin>>jugada;
		cout<<endl;
	    tablero = CreacionTablero(dificultad);
	    valor=GenerarMinas(tablero, dificultad, jugada);
	    if(valor==0){
	    	cout<<"Coordenada fuera de rango, ingrese jugada de nuevo: ";
		}
	    RellenarNumeros(tablero, dificultad);
	}while(valor==0);
	bool primeraJugada=true;
    do{
	    string linea;
	    int valor;
	    if(!primeraJugada){
	    	cout<<"Ingrese jugada : ";
	    	cin>>jugada;
		}else{
			primeraJugada=false;
		}
    	for(int i=0;i<jugada.length();i++){
    	jugada[i]=tolower(jugada[i]);
		}
		if(jugada=="bandera"){
			cin.ignore();
			getline(cin,linea);
		}else{
			linea=jugada;
		}
		stringstream ss(linea);
		string filaStr, columnaStr;
		int fila, columna;
		getline(ss,filaStr,',');
		getline(ss, columnaStr);
		fila=stoi(filaStr)-1;
		columna=stoi(columnaStr)-1;
		bool jugadaValida=verificarJugada(dificultad, tableroVisual, fila, columna);
		if(jugadaValida==1){
			valor=imprimirTableroVisual(dificultad, tablero, tableroVisual,fila,columna,jugada);
			if(valor==0){
				cout<<"Has perdido, presionaste una mina"<<endl;
				ImprimirTablero(dificultad, tableroVisual, tablero, valor);
				return 0;
			}
		}
		else{
			cout<<"Error, coordenada incorrecta"<<endl;
			jugadaValida=verificarJugada(dificultad, tableroVisual, fila, columna);	
		}
	}while(valor==1);
}

