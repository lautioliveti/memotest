#ifndef MODELO_H_INCLUDED
#define MODELO_H_INCLUDED

#include <SDL2/SDL.h>
#include "vector.h"
#include "configuracion.h"

typedef enum { ESCONDIDA=0, REVELADA=1, ENCONTRADA=2 } EstadoFigura;

typedef struct {
    int parID;
    int puntos;// puntos asignados a esa pareja (igual para las dos cartas)
    EstadoFigura estado;// escondido/revelado/encontrado
}Figura;

typedef struct {
    int puntaje;
    int racha;
    int mejorRacha;
    int intentos;// cada 2 selecciones = 1 intento
    int cantEncontrados;// pares acertados
    Uint32 startTimeMs; //REVISAR SI VA (me lo dijo chatgpt)
}Estadisticas;


typedef struct {
    int filas;
    int columnas;
    int numPares;
    Vector figuras;// Vector de figuras, tamaño fila*columna

    int primerIndice;// -1 si nada
    int segundoIndice;// -1 si nada

    int ocultar;// hay que ocultar por fallo
    Uint32 hideAtMs;// momento para ocultar. REVISAR SI VA (me lo dijo chatgpt)
}Tablero;

typedef struct {
    Configuracion cfg;

    int cantJugadores;
    int jugadorActual;
    Jugador jugadores[2];

    Tablero tablero;
}Juego;


void intercambiarFiguras(Figura* a, Figura* b);
void mezclarRandom(Vector* v);
int inicializarTablero(Tablero* b, int filas, int columnas);
void destruirTablero(Tablero* b);

#endif // MODELO_H_INCLUDED
