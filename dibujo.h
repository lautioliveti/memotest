#ifndef DIBUJO_H_INCLUDED
#define DIBUJO_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


void mostrarTexto(SDL_Renderer *renderer, char *mensaje, TTF_Font *font, int x, int y, SDL_Color color);
void mostrarPresentacion(SDL_Renderer *renderer, SDL_Texture *texturaFondo, SDL_Texture *texturaTitulo);
void esperar(int duracion);

#endif // DIBUJO_H_INCLUDED
