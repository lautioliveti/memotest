#ifndef CONFIGURACION_H_INCLUDED
#define CONFIGURACION_H_INCLUDED



#define MENU 1
#define CONFIGURACION 2
#define JUGAR 3
#define ESTADISTICAS 4
#define SALIR 0

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define SET_A 0
#define SET_B 1
#define TAM_NOMBRE 4


typedef struct {
    int filas;
    int columnas;
    int setDeCartas;
    int cantJugadores;
} Configuracion;

typedef struct {
    char nombre[TAM_NOMBRE];
    int puntaje;
} Jugador;

void solicitarNombreJugador(SDL_Renderer *renderer, SDL_Texture *texturaFondo, Jugador* jugador,TTF_Font *font,int numJugador);
int mostrarMenuConfiguracion(SDL_Renderer* renderer, SDL_Texture* texturaFondo, Configuracion *config, TTF_Font *font, SDL_Texture* tIzq, SDL_Texture* tDer);
void guardarConfiguracion(Configuracion *config);
void cargarConfiguracion(Configuracion *config);
#endif // CONFIGURACION_H_INCLUDED
