#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include <SDL2/SDL.h>
#include "configuracion.h"
#include "vector.h"

// Estados de una carta
typedef enum {
    ESTADO_OCULTA = 0,
    ESTADO_REVELADA = 1,
    ESTADO_ENCONTRADA = 2
} EstadoCarta;

// Estructura de una carta
typedef struct {
    int idFigura;          // ID de la figura (0 a N)
    int puntos;            // Puntos que otorga este par
    EstadoCarta estado;    // Estado actual de la carta
    SDL_Rect rect;         // Posición y tamaño en pantalla
} Carta;

// Estado del juego
typedef struct {
    Vector *tablero;           // Vector dinámico de cartas
    int filas;                 // Filas del tablero
    int columnas;              // Columnas del tablero
    int totalCartas;           // Total de cartas
    int paresEncontrados;      // Cantidad de pares encontrados
    int totalPares;            // Total de pares a encontrar
    int primeraCarta;          // Índice de primera carta seleccionada (-1 si ninguna)
    int segundaCarta;          // Índice de segunda carta seleccionada (-1 si ninguna)
    int racha;                 // Aciertos consecutivos
    int esperando;             // Flag para delay
    Uint32 tiempoEspera;       // Timestamp para delay
    SetFiguras setActual;      // Set de figuras en uso
} EstadoJuego;

// Funciones del juego
EstadoJuego* crearJuego(Configuracion *config);
void inicializarTablero(EstadoJuego *juego, Configuracion *config);
int manejarClickCarta(EstadoJuego *juego, DatosJugadores *jugadores, int x, int y);
void actualizarJuego(EstadoJuego *juego, DatosJugadores *jugadores);
int juegoTerminado(EstadoJuego *juego);
void destruirJuego(EstadoJuego *juego);
int obtenerCartaEnPosicion(EstadoJuego *juego, int x, int y);

#endif // JUEGO_H_INCLUDED
