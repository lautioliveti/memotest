#ifndef SONIDO_H_INCLUDED
#define SONIDO_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

// Estructura para almacenar los efectos de sonido
typedef struct {
    Mix_Chunk *seleccion;      // Sonido al seleccionar carta
    Mix_Chunk *acierto;        // Sonido al encontrar par
    Mix_Chunk *error;          // Sonido al fallar
} EfectosSonido;

// Funciones de manejo de sonido
int inicializarSonido();
int cargarEfectosSonido(EfectosSonido *efectos);
void liberarEfectosSonido(EfectosSonido *efectos);
void reproducirSonido(Mix_Chunk *efecto);
void cerrarSonido();

#endif // SONIDO_H_INCLUDED
