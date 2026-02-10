#include "dibujo.h"

void mostrarPresentacion(SDL_Renderer *renderer, SDL_Texture *texturaFondo, SDL_Texture *texturaTitulo) {
    // posicion y dimension del titulo
    int x = 150, y= 200, w = 500, h = 180;


    SDL_RenderClear(renderer);


    SDL_RenderCopy(renderer, texturaFondo, NULL, NULL);


    SDL_Rect rectTitulo = { x, y, w, h };
    SDL_RenderCopy(renderer, texturaTitulo, NULL, &rectTitulo);


    SDL_RenderPresent(renderer);


    esperar(2000);

}


void mostrarTexto(SDL_Renderer *renderer, char *mensaje, TTF_Font *font, int x, int y, SDL_Color color) {
    SDL_Surface *surf = TTF_RenderText_Blended(font, mensaje, color);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_Rect rect = {x, y, surf->w, surf->h};
    SDL_RenderCopy(renderer, tex, NULL, &rect);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}

void esperar(int duracion)
{
    SDL_Event e;
    size_t inicio = SDL_GetTicks();
    while (SDL_GetTicks() - inicio < duracion)
    {
        while (SDL_PollEvent(&e))
            {

            }
    }
}

