#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "modelo.h"
#include "sonido.h"
#include "configuracion.h"

// Estructura para almacenar texturas de cartas
typedef struct {
    SDL_Texture** texturas;  // Array de texturas de figuras
    SDL_Texture* dorso;       // Textura del reverso
    int cantidad;             // Cantidad de texturas cargadas
} TexturasCartas;

// Funciones principales del juego
int jugarPartida(SDL_Renderer *renderer, SDL_Texture *texturaFondo, Configuracion *cfg, Jugador *j1, Jugador *j2, TTF_Font *font,EfectosSonido *efectos);

// Funciones de carga de recursos
int cargarTexturasCartas(SDL_Renderer *renderer, TexturasCartas *tc, int setDeCartas, int numPares);
void liberarTexturasCartas(TexturasCartas *tc);

// Funciones de renderizado
void dibujarTablero(SDL_Renderer *renderer, Tablero *tablero, TexturasCartas *tc, int offsetX, int offsetY, int anchoC, int altoC, int mouseX, int mouseY);
void dibujarHUD(SDL_Renderer *renderer, TTF_Font *font, Jugador *j1, Jugador *j2, int jugadorActual, int cantJugadores, int paresEncontrados, int totalPares, int racha);

// Funciones de lógica de juego
int obtenerCartaCliqueada(int mouseX, int mouseY, Tablero *tablero, int offsetX, int offsetY, int anchoC, int altoC);
void procesarSeleccion(Tablero *tablero, int indice, Jugador *jugadorActivo, int *jugadorActual, int cantJugadores,EfectosSonido *efectos);
void actualizarOcultamiento(Tablero *tablero);
int contarParesEncontrados(Tablero *tablero);


#endif // JUEGO_H_INCLUDED
