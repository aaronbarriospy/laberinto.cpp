Laberinto Maestro - Generador y Resolutor en C++

Este proyecto es una implementacion en C++ puro de un sistema capaz de generar laberintos aleatorios totalmente resolubles y encontrar el camino hacia la salida de forma automatica.

1. ¿Como se genero el laberinto?
Para la generacion, utilice el algoritmo Recursive Backtracker (una variante de la Busqueda en Profundidad o DFS). 
El proceso logico es el siguiente:
- El mapa inicia como una matriz bidimensional (cuadricula) completamente llena de muros (#).
- El algoritmo se posiciona en una celda inicial y elige una direccion al azar (usando arreglos direccionales para Arriba, Derecha, Abajo, Izquierda).
- Avanza "excavando" caminos (dejando espacios en blanco) y saltando de dos en dos celdas para asegurar que siempre quede un muro de un bloque de grosor entre los pasillos.
- Este enfoque garantiza matematicamente que el laberinto sea "perfecto": todas las celdas alcanzables estan conectadas y existe una unica ruta correcta desde la Entrada (E) hasta la Salida (S), sin bucles cerrados.

2. ¿Que algoritmo se uso para resolverlo?
Para encontrar la salida, implemente el algoritmo de Backtracking (Busqueda con Retroceso).
- El programa explora la cuadricula marcando cada paso valido con un asterisco (*).
- Utiliza un bucle for para evaluar las cuatro direcciones posibles en cada interseccion.
- Si el algoritmo se topa con un muro (#) o llega a un callejon sin salida, "retrocede" (borra el asterisco que acaba de poner) y vuelve al ultimo cruce disponible para intentar un camino diferente.
- Continua este proceso de ensayo y error logico hasta que sus coordenadas coinciden con la celda de salida.

3. Descubrimientos al medir tiempos y futuras mejoras
Al utilizar la libreria <chrono> para medir la ejecucion en milisegundos, descubri lo siguiente:
- Tiempos de ejecucion: Ambos procesos son extremadamente rapidos en matrices pequenas (ej. 21x21). Sin embargo, la resolucion suele tomar un poco menos de tiempo que la generacion. Esto ocurre porque la generacion debe visitar y calcular todas las celdas del mapa obligatoriamente, mientras que la resolucion se detiene en el instante en que encuentra la salida, sin necesidad de explorar los callejones restantes.
- ¿Que haria distinto la proxima vez?
  1. Para laberintos de tamano masivo (ej. 1000x1000), el enfoque recursivo actual podria saturar la memoria (Stack Overflow). La proxima vez implementaria una estructura iterativa utilizando el concepto de "Pilas" (std::stack).
  2. El Backtracking actual encuentra el primer camino valido, pero no evalua si es el mas corto. Para garantizar la ruta mas eficiente, cambiaria el enfoque a un algoritmo de Busqueda a lo Ancho (BFS) utilizando "Colas" (std::queue).