#include <iostream>   // Traigo la herramienta iostream para poder imprimir en mi pantalla.
#include <vector>     // Uso vector porque es como una caja elastica, mejor que las listas antiguas de C.
#include <cstdlib>    // Necesito esto para usar rand() y poder sacar numeros al azar.
#include <ctime>      // Traigo el reloj de la compu para que mis laberintos siempre sean unicos.
#include <chrono>     // Mi cronometro para medir cuanto tardo en cumplir el reto.
#include <string>     // Para poder usar textos largos sin problemas.

using namespace std;  // Uso este atajo para no tener que escribir "std::" antes de cada orden.

// ---------------------------------------------------------
// 1. MIS REGLAS VISUALES (CONSTANTES GLOBALES)
// ---------------------------------------------------------
// Aqui defino mis piezas de lego. Uso 'const' para que nadie me las cambie por accidente.
const char MURO = '#';   // Este es el simbolo de mis paredes.
const char CAMINO = ' '; // Espacio vacio por donde voy a caminar.
const char RUTA = '*';   // Las migajas de pan que voy a dejar para cuando encuentre la salida.
const char INICIO = 'E'; // E de Entrada.
const char SALIDA = 'S'; // S de Salida.

// ---------------------------------------------------------
// 2. MIS MOVIMIENTOS (TRUCO MATEMATICO)
// ---------------------------------------------------------
// En lugar de hacer mil preguntas con 'if', uso estas dos listas para girar.
// Indice 0: Arriba (0 en X, -1 en Y)
// Indice 1: Derecha (1 en X, 0 en Y)
// Indice 2: Abajo (0 en X, 1 en Y)
// Indice 3: Izquierda (-1 en X, 0 en Y)
int dir_x[] = {0, 1, 0, -1};
int dir_y[] = {-1, 0, 1, 0};

// ---------------------------------------------------------
// 3. LA FABRICA (MI CLASE PRINCIPAL)
// ---------------------------------------------------------
class SistemaLaberinto { // Establezco la clase para construir el laberinto. Son como planos de construccion
private:
    int ancho, alto;                   // Cajas para guardar el tamano que quiero para mi mapa.
    vector<vector<char>> cuadricula;   // Aca creo un mapa bidimensional(matriz) en donde cada celdita guarda un solo caracter.

    // Mi guardia de seguridad: me avisa si estoy intentando salirme del mapa.
    bool esValido(int x, int y) {
        // Devuelvo verdadero solo si mis coordenadas estan dentro de los limites permitidos.
        return (x > 0 && x < ancho - 1 && y > 0 && y < alto - 1);
    }

    // ---------------------------------------------------------
    // GENERACION: Mi algoritmo Recursive Backtracker (DFS)
    // ---------------------------------------------------------
    void generarDFS(int x, int y) { /// Labura pero no devuelve numero como resultado.
        cuadricula[y][x] = CAMINO; // Piso la celda actual y la convierto en un pasillo libre.

        int direcciones[] = {0, 1, 2, 3}; // Preparo mis 4 opciones de giro.

        // Mezclo mis direcciones al azar para que el laberinto no sea aburrido y predecible.
        for (int i = 0; i < 4; i++) {
            int aleatorio = rand() % 4;                   // Saco un numero del 0 al 3.
            swap(direcciones[i], direcciones[aleatorio]); // Intercambio la posicion actual con la que salio al azar.
        }

        // Ahora intento avanzar hacia las 4 direcciones ya mezcladas.
        for (int i = 0; i < 4; i++) {
            // Multiplico por 2 porque para cavar necesito saltar un muro y dejar la pared de por medio.
            int nuevo_x = x + dir_x[direcciones[i]] * 2;
            int nuevo_y = y + dir_y[direcciones[i]] * 2;

            // Le pregunto a mi guardia si el salto es valido y si aterrice en un muro virgen.
            if (esValido(nuevo_x, nuevo_y) && cuadricula[nuevo_y][nuevo_x] == MURO) {
                // Si todo esta bien, derribo el muro que quedo en el medio de mi salto.
                cuadricula[y + dir_y[direcciones[i]]][x + dir_x[direcciones[i]]] = CAMINO;

                // Uso recursividad: me llamo a mi mismo desde la nueva posicion para seguir cavando.
                generarDFS(nuevo_x, nuevo_y);
            }
        }
    }

    // ---------------------------------------------------------
    // RESOLUCION: Mi algoritmo Backtracking (Ensayo y Error)
    // ---------------------------------------------------------
    bool resolverBacktracking(int x, int y) {
        // CASO BASE: Compruebo si ya llegue a la esquina inferior derecha (mi meta).
        if (x == ancho - 2 && y == alto - 2) {
            cuadricula[y][x] = RUTA; // Marco que pise la salida.
            return true;             // Grito "victoria" y termino la busqueda hacia atras.
        }

        // Solo avanzo si la celda es un camino libre o si es la entrada.
        if (cuadricula[y][x] == CAMINO || cuadricula[y][x] == INICIO) {

            // Dejo mi migaja de pan (el asterisco) para marcar la ruta que estoy probando.
            // Pongo un if para asegurarme de no borrar mi E de entrada.
            if (cuadricula[y][x] != INICIO) cuadricula[y][x] = RUTA;

            // Miro hacia mis 4 direcciones posibles.
            for (int i = 0; i < 4; i++) {
                int nuevo_x = x + dir_x[i]; // Aqui avanzo de a 1, ya no multiplico por 2.
                int nuevo_y = y + dir_y[i];

                // Doy el paso. Si este camino eventualmente me lleva al final, devuelvo verdadero.
                if (resolverBacktracking(nuevo_x, nuevo_y)) {
                    return true;
                }
            }

            // BACKTRACKING: Si intente por todos lados y no hay salida, recojo mi migaja y retrocedo.
            if (cuadricula[y][x] != INICIO) cuadricula[y][x] = CAMINO;
        }

        // Si choque con pared o con mi propia ruta, aviso que por aqui no es.
        return false;
    }

public:
    // El constructor: se ejecuta apenas creo mi sistema de laberinto.
    SistemaLaberinto(int ancho_deseado, int alto_deseado) {
        // El algoritmo exige que el mapa sea impar. Si me pasan un numero par, lo arreglo sumandole 1.
        ancho = (ancho_deseado % 2 == 0) ? ancho_deseado + 1 : ancho_deseado;
        alto = (alto_deseado % 2 == 0) ? alto_deseado + 1 : alto_deseado;

        // Relleno toda mi matriz con muros solidos antes de empezar a cavar.
        cuadricula.assign(alto, vector<char>(ancho, MURO));
    }

    // Boton publico para generar.
    void generarLaberinto() {
        srand(time(NULL)); // Enciendo la aleatoriedad vinculandola con la hora actual.
        generarDFS(1, 1);  // Empiezo a picar desde la coordenada 1,1.

        cuadricula[1][1] = INICIO;                 // Pongo la E de entrada.
        cuadricula[alto - 2][ancho - 2] = SALIDA;  // Pongo la S de salida.
    }

    // Boton publico para resolver.
    void resolverLaberinto() {
        resolverBacktracking(1, 1); // Empiezo a buscar la salida desde mi entrada en 1,1.

        // Vuelvo a pintar la E y la S por si mi algoritmo las piso por accidente.
        cuadricula[1][1] = INICIO;
        cuadricula[alto - 2][ancho - 2] = SALIDA;
    }

    // Boton publico para mostrar mi obra de arte en la consola.
    void mostrarLaberinto() {
        for (int y = 0; y < alto; y++) {
            for (int x = 0; x < ancho; x++) {
                cout << cuadricula[y][x] << ' '; // Imprimo celda por celda y le sumo un espacio para separarlas.
            }
            cout << '\n'; // Salto de linea al terminar cada fila.
        }
        // Dibujo una linea separadora usando guiones en la parte inferior.
        cout << string(ancho * 2, '-') << '\n';
    }
};

// ---------------------------------------------------------
// 4. EL JEFE DEL PROGRAMA (MAIN)
// ---------------------------------------------------------
int main(int argc, char* argv[]) {
    // argc me dice cuantos parametros me pasaron por la consola de MSYS2.
    // argv es la lista con el texto de esos parametros.

    int ancho_final = 10;  // Mi tamano por defecto si decido no escribir parametros en la terminal.
    int alto_final = 10;

    // Si ejecuto el programa en la consola como './laberinto 15 15', atrapo esos numeros aqui.
    if (argc >= 3) {
        ancho_final = stoi(argv[1]);  // stoi = String TO Integer (convierto el texto a un numero).
        alto_final = stoi(argv[2]);
    }

    cout << "Inicializando mi Laberinto Maestro (" << ancho_final << "x" << alto_final << ")...\n";
    SistemaLaberinto miLaberinto(ancho_final, alto_final); // Construyo mi objeto laberinto con las medidas dadas.

    // --- MIDO MI TIEMPO DE GENERACION ---
    auto inicioGen = chrono::high_resolution_clock::now(); // Prendo mi cronometro.
    miLaberinto.generarLaberinto();                        // Genero mi mapa al azar.
    auto finGen = chrono::high_resolution_clock::now();    // Apago el cronometro.
    chrono::duration<double, milli> tiempoGen = finGen - inicioGen; // Calculo la diferencia en milisegundos.

    cout << "Laberinto Generado:\n";
    miLaberinto.mostrarLaberinto(); // Lo imprimo vacio para verlo.

    // --- MIDO MI TIEMPO DE RESOLUCION ---
    auto inicioRes = chrono::high_resolution_clock::now(); // Prendo el cronometro otra vez.
    miLaberinto.resolverLaberinto();                       // Lanzo el algoritmo para que busque la salida.
    auto finRes = chrono::high_resolution_clock::now();    // Apago el cronometro.
    chrono::duration<double, milli> tiempoRes = finRes - inicioRes; // Saco el tiempo final.

    cout << "Laberinto Resuelto:\n";
    miLaberinto.mostrarLaberinto(); // Imprimo el mapa final con la ruta resuelta.

    // Imprimo mis metricas para el entregable.
    cout << "Tiempo de generacion: " << tiempoGen.count() << " ms\n";
    cout << "Tiempo de resolucion: " << tiempoRes.count() << " ms\n";

    return 0; // Termino mi programa sin errores.
}