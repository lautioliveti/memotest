#include "dibujo.h"

void mostrarPresentacion(SDL_Renderer *renderer, SDL_Texture *texturaFondo, SDL_Texture *texturaTitulo, TTF_Font *font) {
    // posicion y dimension del titulo
    SDL_Color blanco = {255, 255, 255, 255};
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texturaFondo, NULL, NULL);
    SDL_Rect rectTitulo = { 150, 150, 500, 180 };
    SDL_RenderCopy(renderer, texturaTitulo, NULL, &rectTitulo);
    mostrarTexto(renderer,"Presiona una tecla para empezar", font,140,350,blanco);

    SDL_Event e;
    int corriendo=1;

    while(corriendo)
    {
        while(SDL_PollEvent(&e))
        {
            if (e.type == SDL_KEYDOWN) {
                corriendo = 0;
            }
        }
        SDL_RenderPresent(renderer);

    }
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

