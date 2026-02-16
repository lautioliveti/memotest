#ifndef ESTADISTICAS_H_INCLUDED
#define ESTADISTICAS_H_INCLUDED

#include "configuracion.h"
#include "Vector.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define TOP_MAX 10

typedef struct {
    char nombre[TAM_NOMBRE];
    int puntaje;
} EntradaTop;

typedef struct {
    Vector entradas;  // Vector de EntradaTop
    int cantidad;     // Cantidad actual de entradas
} TopSesion;

// Funciones de gestion del Top
int inicializarTopSesion(TopSesion* top);
void destruirTopSesion(TopSesion* top);
int agregarEntradaTop(TopSesion* top, const char* nombre, int puntaje);
void ordenarTop(TopSesion* top);

// Funcion para mostrar el Top 10
int mostrarEstadisticas(SDL_Renderer* renderer, SDL_Texture* texturaFondo,
                        TopSesion* top, TTF_Font* font, TTF_Font* fontSmall);

// Funciones de archivos para estadísticas
int cargarTopDesdeArchivo(TopSesion* top, const char* path);
int guardarTopEnArchivo(const TopSesion* top, const char* path);

#endif // ESTADISTICAS_H_INCLUDED
