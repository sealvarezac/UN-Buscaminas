#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <algorithm>

using namespace std;
// FUNCIONES DE CREACIÓN Y MANEJO DEL TABLERO

/*
 * Crea una matriz dinámica de enteros para la lógica interna del juego.
 * Inicializa todas las casillas en 0.
 */
int** CreacionTablero (int tamano){
    int** tablero;
    tablero = new int*[tamano];
    for(int i = 0; i < tamano; i++){
        tablero[i] = new int[tamano];
        for(int j = 0; j < tamano; j++){
            tablero[i][j] = 0;
        }
    }          
    return tablero;
}

/*
 * Crea una matriz dinámica de caracteres (unsigned char) para lo que ve el jugador.
 * Inicializa todas las casillas con el carácter 178 (bloque oculto).
 */
unsigned char** crearTableroVisual (int tamano){
    unsigned char** tableroVisual = new unsigned char*[tamano];
    for(int i = 0; i < tamano; i++){
        tableroVisual[i] = new unsigned char[tamano];
        for(int j = 0; j < tamano; j++){
            tableroVisual[i][j] = 178; // Código ASCII para el bloque sombreado
        }
    }
    return tableroVisual;
}

/*
 * Verifica si una coordenada (fila, columna) está dentro de los límites del tablero.
 */
bool Rango (int fila, int columna, int tamano){
    if(fila >= 0 && fila < tamano && columna >= 0 && columna < tamano){
        return true;
    }else{
        return false;
    }
}

// LÓGICA DE MINAS Y NÚMEROS

/*
 * Distribuye las minas aleatoriamente en el tablero interno.
 * Garantiza que el primer clic (y sus alrededores) estén libres de minas 
 * para que el jugador no pierda en el primer turno.
 */
void GenerarMinas (int** tablero, int tam, int fila, int columna, int totalMinas){
    int FilaAleatoria = 0;
    int ColumnaAleatoria = 0;
    int MinasPuestas = 0;
    
    if(Rango(fila, columna, tam)){
        // 1. Marca la zona del primer clic temporalmente (con 1s) para protegerla
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                int nx = fila + i;
                int ny = columna + j;
                if (Rango(nx, ny, tam)) {
                    tablero[nx][ny] = 1;
                }
            }
        }
        
        // 2. Coloca las minas (representadas con el número 10)
        while(MinasPuestas < totalMinas){
            FilaAleatoria = rand() % tam;
            ColumnaAleatoria = rand() % tam;
            // Solo pone mina si no hay otra mina (10) y si no es la zona protegida (1)
            if(tablero[FilaAleatoria][ColumnaAleatoria] != 10 && tablero[FilaAleatoria][ColumnaAleatoria] != 1){
                tablero[FilaAleatoria][ColumnaAleatoria] = 10;
                MinasPuestas++;    
            }
        }    
    }
}

/*
 * Cuenta cuántas minas hay en las 8 casillas alrededor de una coordenada específica.
 */
int MinasAlrededor(int** tablero, int fila, int columna, int tam) {
    int contador = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int nx = fila + i;
            int ny = columna + j;
            if (Rango(nx, ny, tam)) {
                if (tablero[nx][ny] == 10) { // Si hay una mina
                    contador++;
                }
            }
        }
    }
    return contador;
}

/*
 * Recorre todo el tablero y asigna a cada casilla (que no sea mina) 
 * el número de minas que tiene alrededor.
 */
void RellenarNumeros(int** tablero, int tam) {
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            if (tablero[i][j] != 10) { // Si no es mina (10)
                tablero[i][j] = MinasAlrededor(tablero, i, j, tam);
            }
        }
    }
}

// FUNCIONES VISUALES Y JUGABILIDAD

/*
 * Algoritmo recursivo (Flood Fill) que destapa automáticamente las casillas vacías (0)
 * y sus bordes numéricos, simulando el efecto en cadena del Buscaminas.
 */
void floodFill (int** tablero, unsigned char** tableroVisual, int fila, int columna, int tam, int& banderas){
    if(tablero[fila][columna] == 0){
        // Convierte el número entero a su representación en char ('0')
        tableroVisual[fila][columna] = tablero[fila][columna] + '0';
        
        // Revisa los 8 vecinos
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                int nx = fila + i;
                int ny = columna + j;
                
                if (Rango(nx, ny, tam)) {
                    // Si el vecino está oculto o tiene una bandera
                    if(tableroVisual[nx][ny] == 178 || tableroVisual[nx][ny] == 201){
                        // Si había una bandera, la devolvemos al contador
                        if(tableroVisual[nx][ny] == 201){
                            banderas++;
                        }
                        // Si es un número del 1 al 8, solo lo destapa
                        if(tablero[nx][ny] > 0 && tablero[nx][ny] <= 8){
                            tableroVisual[nx][ny] = tablero[nx][ny] + '0';
                        } 
                        // Si es otro 0, hace la llamada recursiva para seguir destapando
                        else if(tablero[nx][ny] == 0){    
                            tableroVisual[nx][ny] = tablero[nx][ny] + '0';
                            floodFill(tablero, tableroVisual, nx, ny, tam, banderas);
                        }
                    }
                }
            }
        }
    }
}

/*
 * Procesa la jugada del usuario y actualiza la matriz visual.
 * Imprime el tablero en consola cada vez que se llama.
 */
void imprimirTableroVisual(int tamano, int** tablero, unsigned char** tableroVisual, int fila, int columna, string jugada, int& banderas){
    // Recupera banderas si se hace clic sobre una ya puesta
    if(tableroVisual[fila][columna] == 201) {
        banderas++;
    }
    
    // Lógica para poner/quitar bandera (201 en ASCII)
    if(jugada == "bandera"){
        tableroVisual[fila][columna] = 201;
    } 
    // Lógica si el jugador pisa una mina (Muestra todas las minas)
    else if(tablero[fila][columna] == 10){
        for (int i = 0; i < tamano; i++) {
            for (int j = 0; j < tamano; j++) {
                if(tablero[i][j] == 10){
                    tableroVisual[i][j] = 158; // Carácter para la mina explotada
                }
                cout << tableroVisual[i][j] << "  ";
            }
            cout << endl << endl;
        }
        return; // Termina la función para evitar imprimir el tablero dos veces
    } 
    // Si pisa un área vacía, llama a la recursividad
    else if(tablero[fila][columna] == 0){
        floodFill(tablero, tableroVisual, fila, columna, tamano, banderas);
    } 
    // Si pisa un número solitario, solo lo destapa
    else {
        tableroVisual[fila][columna] = tablero[fila][columna] + '0';
    }
    
    // --- IMPRESIÓN DEL TABLERO ACTUALIZADO ---
    cout << "  ";
    for(int a = 1; a <= tamano; a++){ // Imprime coordenadas superiores
        if(a < 10){
            cout << a << "  ";
        } else {
            cout << a << " ";
        }
    }
    cout << endl;
    
    for (int i = 0; i < tamano; i++) {
        if(i + 1 < 10){ // Imprime coordenadas laterales
            cout << i + 1 << " ";
        } else {
            cout << i + 1 << "";
        }
        
        for (int j = 0; j < tamano; j++) {
            cout << tableroVisual[i][j] << "  ";
        }
        cout << endl << endl;
    }
    cout << "Banderas: " << banderas << endl;
}

/*
 * Muestra el tablero al inicio del juego o justo después de cargar una partida.
 */
void mostrarTableroInicial(int tamano, unsigned char** tableroVisual, int& banderas) {
    cout << "  ";
    for(int a = 1; a <= tamano; a++){
        if(a < 10){
            cout << a << "  ";
        } else {
            cout << a << " ";
        }
    }
    cout << endl;
    
    for (int i = 0; i < tamano; i++) {
        if(i + 1 < 10){
            cout << i + 1 << " ";
        } else {
            cout << i + 1 << "";
        }
        for (int j = 0; j < tamano; j++) {
            cout << tableroVisual[i][j] << "  ";
        }
        cout << endl << endl;
    }
    cout << "Banderas: " << banderas << endl;
}

// ESTADOS DE JUEGO

/*
 * Revisa si una coordenada específica ya está destapada para evitar jugadas redundantes.
 */
bool verificarJugada(unsigned char** tableroVisual, int fila, int columna, int tamano){
    if(Rango(fila, columna, tamano) && (tableroVisual[fila][columna] == 178 || tableroVisual[fila][columna] == 201)){
        return true;
    }else{
        return false;
    }
}

/*
 * Comprueba si el jugador destapó todas las casillas seguras, otorgando la victoria.
 */
bool validarVictoria(unsigned char** tableroVisual, int** tablero, int tamano, int minas){
    int destapadas = (tamano * tamano) - minas;
    int contar = 0;
    
    // Cuenta cuántas casillas diferentes al bloque (178) y banderas (201) hay
    for(int i = 0; i < tamano; i++){
        for(int j = 0; j < tamano; j++){
            if(tablero[i][j] != 10 && tableroVisual[i][j] != 178 && tableroVisual[i][j] != 201){
                contar++;
            }
        }
    }
    
    if(contar == destapadas){
        return true;
    }else{
        return false;
    }
}

/*
 * Verifica si las coordenadas ingresadas coinciden con una mina.
 */
bool pisoMina(int** tablero, int fila, int columna){
    if(tablero[fila][columna] == 10){
        return true;
    }else{
        return false;
    }
}

/*
 * Mantiene el bucle principal corriendo mientras no haya victoria ni derrota.
 */
bool juegoActivo(bool gano, bool perdio){
    if(gano || perdio){
        return false;
    }else{
        return true;
    }
}

// SISTEMA DE GUARDADO

/*
 * Guarda las dimensiones, configuración, matriz interna y visual en un archivo .txt.
 */
bool guardarPartida(int** tablero, unsigned char** tableroVisual, int tamano, int minas, int banderas, string partida){
    string ruta = "Partidas/" + partida + ".txt";
    ofstream Archivo(ruta);
    if(Archivo.is_open()){
        // Guardar configuración
        Archivo << tamano << endl;
        Archivo << minas << endl;
        Archivo << banderas << endl << endl;
        
        // Guardar lógica interna
        for(int i = 0; i < tamano; i++){
            for(int j = 0; j < tamano; j++){
                Archivo << tablero[i][j] << " ";
            }
            Archivo << endl;
        }
        Archivo << endl << endl;
        
        // Guardar progreso visual
        for(int i = 0; i < tamano; i++){
            for(int j = 0; j < tamano; j++){
                Archivo << tableroVisual[i][j] << " ";
            }
            Archivo << endl;
        }
        return true;
    }else{
        return false;
    }
}

/*
 * Lee el archivo .txt de guardado y reconstruye ambas matrices para continuar jugando.
 */
bool cargarPartida(string partida, int& banderas, int& minas, int& tam, int**& tablero, unsigned char**& tableroVisual) {
    string ruta = "Partidas/" + partida + ".txt";
    ifstream Archivo(ruta); 
    
    if (Archivo.is_open()) {
        Archivo >> tam;
        Archivo >> minas;
        Archivo >> banderas;
        
        tablero = CreacionTablero(tam);
        tableroVisual = crearTableroVisual(tam);
        
        for (int i = 0; i < tam; i++) {
            for (int j = 0; j < tam; j++) {
                Archivo >> tablero[i][j];
            }
        }
        
        for (int i = 0; i < tam; i++) {
            for (int j = 0; j < tam; j++) {
                Archivo >> tableroVisual[i][j]; 
            }
        }
        return true;
    } else {
        return false;
    }
}

// HISTÓRICO Y PUNTAJES

/*
 * Lee los récords guardados, inserta el nuevo, ordena con el Método Burbuja (Bubble Sort) 
 * y reescribe solo los 3 mejores al archivo.
 */
void actualizarHistorico(string nombre, int puntaje) {
    string nombres[4] = {"", "", "", ""};
    int puntos[4] = {0, 0, 0, 0};
    
    // Posicionamos el récord actual al final del arreglo temporal
    nombres[3] = nombre;
    puntos[3] = puntaje;
    
    ifstream ArchivoEntrada("HistoricoPuntajes/Puntajes.txt");
    string linea;
    int indice = 0; 
    
    if (ArchivoEntrada.is_open()) {
        while (getline(ArchivoEntrada, linea) && indice < 3) {
            stringstream ss(linea);
            string nombreExtraido;
            string puntajeExtraido;
            
            // Extrae el texto antes del guion ('-')
            if (getline(ss, nombreExtraido, '-')) {
                getline(ss, puntajeExtraido);
                
                // Limpia el espacio final del nombre
                if (!nombreExtraido.empty() && nombreExtraido.back() == ' ') {
                    nombreExtraido.pop_back();
                }
                
                nombres[indice] = nombreExtraido;
                try {
                    puntos[indice] = stoi(puntajeExtraido);
                } catch (...) {
                    puntos[indice] = 0;
                }
                indice++; 
            }
        }
        ArchivoEntrada.close();
    }
    
    // Método Burbuja: Compara de a parejas para empujar los más pequeños al final
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3 - i; j++) {
            if (puntos[j] < puntos[j + 1]) {
                // Intercambio de puntos
                int tempPunto = puntos[j];
                puntos[j] = puntos[j + 1];
                puntos[j + 1] = tempPunto;
                
                // Intercambio sincronizado de nombres
                string tempNombre = nombres[j];
                nombres[j] = nombres[j + 1];
                nombres[j + 1] = tempNombre;
            }
        }
    }
    
    // Guarda el Top 3 actualizado borrando el archivo viejo
    ofstream ArchivoSalida("HistoricoPuntajes/Puntajes.txt");
    if (ArchivoSalida.is_open()) {
        for (int i = 0; i < 3; i++) {
            if (nombres[i] != "") {
                ArchivoSalida << nombres[i] << " - " << puntos[i] << endl;
            }
        }
        ArchivoSalida.close();
    }
}

/*
 * Lee el archivo de puntajes y lo imprime en pantalla para felicitar al Top 3.
 */
void mostrarHistorico() {
    cout << "\n===============================" << endl;
    cout << "      TOP 3 MEJORES JUGADORES    " << endl;
    cout << "===============================" << endl;

    ifstream ArchivoEntrada("HistoricoPuntajes/Puntajes.txt");
    string linea;
    int posicion = 1;
    
    if (ArchivoEntrada.is_open()) {
        while (getline(ArchivoEntrada, linea)) {
            stringstream ss(linea);
            string nombre;
            string puntaje;

            // Extrae y da formato para imprimir directamente en consola
            if (getline(ss, nombre, '-')) {            
                getline(ss, puntaje);
                if (!nombre.empty() && nombre.back() == ' ') {
                    nombre.pop_back();
                }
                cout << " " << posicion << ". " << nombre << " -" << puntaje << " pts" << endl;
                posicion++;
            }
        }
        ArchivoEntrada.close();
        if (posicion == 1) {
            cout << " Aún no hay registros de partidas." << endl;
        }
    } else {
        cout << " Aún no hay registros de partidas." << endl;
    }
    cout << "===============================\n" << endl;
}

// FUNCIÓN PRINCIPAL

int main(){
    srand(time(0));
    int eleccion = 0;
    int tamano = 0;
    int minas = 0;
    int banderas = 0;
    int puntaje = 0;
    int fila, columna;
    int** tablero;
    bool pisaMina = false;
    bool ganar = false;
    unsigned char** tableroVisual;
    string jugada, filaStr, columnaStr, partida, jugador;
    
    // 1. PANTALLA DE INICIO Y TUTORIAL
    do{
        cout << "=====================================================================\n"
             << "                     ¡BIENVENIDX A UN MINAZO!                        \n"
             << "=====================================================================\n\n"
             << " El objetivo es despejar todo el tablero sin detonar ninguna mina.   \n\n"
             << " 1. CÓMO JUGAR:\n"
             << " Para descubrir una casilla, ingresa la coordenada escribiendo\n"
             << " PRIMERO la Fila y DESPUÉS la Columna (separadas por una coma).\n"
             << " Ejemplo -> '5,4' (Selecciona la Fila 5, Columna 4)\n\n"
             << " 2. REGLA DE LA BANDERA:\n"
             << " Si sospechas de una mina, escribe la palabra 'bandera' seguida de\n"
             << " la coordenada.\n"
             << " Ejemplo -> bandera\n"
             << " Seguido de -> 3,2\n\n"
             << " NOTA: Tienes banderas limitadas. Si te quedas sin ellas,\n"
             << " no podrás poner más hasta que quites una bandera vieja repitiendo\n"
             << " el mismo comando sobre la casilla marcada.\n\n"
             << " 3. GUARDAR PARTIDA:\n"
             << " Si quieres salvar tu progreso para después, escribe la palabra\n"
             << " 'guardar' en tu turno y listo.\n\n"
             << " Escriba 'continuar' si ya quedó entendido.\n\n"
             << "=====================================================================\n" 
             << endl;
             
        cin >> jugada;
        // Convierte el input a minúsculas para validación fácil
        transform(jugada.begin(), jugada.end(), jugada.begin(), ::tolower);
    } while(jugada != "continuar");
    
    // 2. SELECCIÓN DE DIFICULTAD Y CARGADO
    do{
        cout << "Seleccione una dificultad válida:\n"
             << "1. Fácil (Tablero 10 x 10)\n"
             << "2. Medio (Tablero 15 x 15)\n"
             << "3. Difícil (Tablero 20 x 20)\n"
             << "4. Cargar partida" << endl;
        
        cin >> eleccion;
        
        // Manejo de errores por si el usuario introduce una letra
        if(cin.fail()){
            cin.clear();
            while (cin.get() != '\n');
            eleccion = 0; 
        }else{
            switch(eleccion){
            case 1:
                tamano = 10;
                minas = 10;
                banderas = 10;
                break;
            case 2:
                tamano = 15;
                minas = 30;
                banderas = 30;
                break;
            case 3:
                tamano = 20;
                minas = 60;
                banderas = 60;
                break;
            case 4:
                cout << "Ingrese el nombre del archivo que desea cargar: ";
                cin >> partida;
                if(cargarPartida(partida, banderas, minas, tamano, tablero, tableroVisual)){
                    cout << "Archivo cargado con éxito.\n" << endl;    
                    break;
                }else{
                    cout << "Error: Archivo no válido." << endl;
                }
            default:
                eleccion = 0;
                while (cin.get() != '\n');
                eleccion = 0; 
            }    
        }
    } while(eleccion == 0);
    
    // 3. PRIMER TURNO (Protección contra muerte instantánea)
    if(eleccion != 4){
        tablero = CreacionTablero(tamano);
        tableroVisual = crearTableroVisual(tamano);
        mostrarTableroInicial(tamano, tableroVisual, banderas);
        
        do{
            cout << "Ingrese su jugada:" << endl;
            cin >> jugada;
            transform(jugada.begin(), jugada.end(), jugada.begin(), ::tolower);
            
            if(jugada == "bandera"){
                while (cin.get() != '\n');
                cin >> jugada;
                try{
                    stringstream ss(jugada);
                    getline(ss, filaStr, ',');
                    getline(ss, columnaStr);
                    fila = stoi(filaStr) - 1;
                    columna = stoi(columnaStr) - 1;
                    
                    if(Rango(fila, columna, tamano) && banderas > 0){
                        banderas--;
                        imprimirTableroVisual(tamano, tablero, tableroVisual, fila, columna, "bandera", banderas);
                    }else if (banderas <= 0){
                        cout << "Acción inválida, no le quedan banderas disponibles." << endl;
                    }else{
                        cout << "Coordenada inválida." << endl;
                    }
                }catch(...){
                    cout << "Error: Formato inválido." << endl;
                }
            }else if(jugada == "guardar"){
                cout << "Escriba el nombre con el que desea guardar la partida: ";
                cin >> partida;
                guardarPartida(tablero, tableroVisual, tamano, minas, banderas, partida);
                cout << "Partida guardada de manera exitosa." << endl;
            }else{
                // Este es el primer destape del juego, aquí se generan las minas
                stringstream ss(jugada);
                getline(ss, filaStr, ',');
                getline(ss, columnaStr);
                try{
                    fila = stoi(filaStr) - 1;
                    columna = stoi(columnaStr) - 1;
                    if(Rango(fila, columna, tamano)){
                        GenerarMinas(tablero, tamano, fila, columna, minas);
                        RellenarNumeros(tablero, tamano);
                        imprimirTableroVisual(tamano, tablero, tableroVisual, fila, columna, jugada, banderas);
                        break; // Rompe para pasar al bucle principal
                    }else{
                        cout << "Coordenada inválida." << endl;
                    }
                }catch(...){
                    cout << "Error: Formato inválido." << endl;
                }
            }
        } while(true);        
    } else {
        // Si la partida es cargada, omite generar minas nuevas e imprime el tablero
        mostrarTableroInicial(tamano, tableroVisual, banderas);
    }
    
    // 4. BUCLE PRINCIPAL DEL JUEGO
    do{
        cout << "Ingrese su jugada:" << endl;
        cin >> jugada;
        transform(jugada.begin(), jugada.end(), jugada.begin(), ::tolower);
        
        if(jugada == "bandera"){
            while (cin.get() != '\n');
            cin >> jugada;
            try{
                stringstream ss(jugada);
                getline(ss, filaStr, ',');
                getline(ss, columnaStr);
                fila = stoi(filaStr) - 1;
                columna = stoi(columnaStr) - 1;
                
                // Solo deja poner banderas si hay disponibles y si la casilla está sin destapar
                if(Rango(fila, columna, tamano) && banderas > 0 && tableroVisual[fila][columna] == 178){
                    banderas--;
                    imprimirTableroVisual(tamano, tablero, tableroVisual, fila, columna, "bandera", banderas);
                }else if (banderas <= 0){
                    cout << "Acción inválida, no le quedan banderas disponibles." << endl;
                }else{
                    cout << "Coordenada inválida." << endl;
                }
            }catch(...){
                cout << "Error: Formato inválido." << endl;
            }
            
        }else if(jugada == "guardar"){
            cout << "Escriba el nombre con el que desea guardar la partida: ";
            cin >> partida;
            guardarPartida(tablero, tableroVisual, tamano, minas, banderas, partida);
            cout << "Partida guardada de manera exitosa." << endl;
        }else{
            // Jugada normal (destapar)
            stringstream ss(jugada);
            getline(ss, filaStr, ',');
            getline(ss, columnaStr);
            try{
                fila = stoi(filaStr) - 1;
                columna = stoi(columnaStr) - 1;
                if(Rango(fila, columna, tamano)){
                    imprimirTableroVisual(tamano, tablero, tableroVisual, fila, columna, jugada, banderas);
                    // Actualiza estado de victoria/derrota en cada jugada
                    ganar = validarVictoria(tableroVisual, tablero, tamano, minas);
                    pisaMina = pisoMina(tablero, fila, columna);
                }else{
                    cout << "Coordenada inválida." << endl;
                }
            }catch(...){
                cout << "Error: Formato inválido." << endl;
            }
        }
    } while(juegoActivo(ganar, pisaMina));
    
    // 5. CÁLCULO DE PUNTAJES FINAL
    for(int i = 0; i < tamano; i++){
        for(int j = 0; j < tamano; j++){
            // Cuenta casillas destapadas
            if(tableroVisual[i][j] != 178 && tableroVisual[i][j] != 201){
                puntaje++;
            }
        }
    }
    
    if(ganar){
        cout << "¡Felicidades, ganaste!" << endl;
        puntaje = puntaje * 2; // Multiplicador por ganar
    }else{
        cout << "¡Perdiste! :(" << endl;
    }
    
    cout << "Obtuviste " << puntaje << " puntos." << endl;
    cout << "Ingresa tu nombre sin espacios: " << endl;    
    cin >> jugador;
    
    actualizarHistorico(jugador, puntaje);
    mostrarHistorico();
}
