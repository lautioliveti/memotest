#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "configuracion.h"
#include "dibujo.h"
#include "juego.h"

int mostrarMenuInicial(SDL_Renderer *renderer, SDL_Texture *texturaFondo, SDL_Texture *texturaTitulo,TTF_Font *font);

int main(int argc, char* argv[]) {
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return -1;
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    TTF_Init();

    SDL_Window *ventana = SDL_CreateWindow("MemoTest - UNLaM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Texture *texturaFondo = IMG_LoadTexture(renderer, "img/fondo.jpg");
    SDL_Texture *texturaTitulo = IMG_LoadTexture(renderer, "img/titulomemotest.png");

    SDL_Texture *texFlechaIzq = IMG_LoadTexture(renderer, "img/flechaIzq.png");
    SDL_Texture *texFlechaDer = IMG_LoadTexture(renderer, "img/flechaDer.png");
    if (!texFlechaIzq || !texFlechaDer)
    {
        printf("Error cargando flechas: %s\n", IMG_GetError());
        return -1;
    }
    TTF_Font *font = TTF_OpenFont("fnt/BitcountGridSingle_Cursive-Regular.ttf", 32);
    if (!font)
        return -1;


    Configuracion miConfig;
    cargarConfiguracion(&miConfig); // inicializo config

    Jugador j1, j2;

    int estadoActual = MENU;
    int corriendo = 1;

    mostrarPresentacion(renderer, texturaFondo, texturaTitulo);

    while (corriendo) {
        switch (estadoActual) {
            case MENU:

                estadoActual = mostrarMenuInicial(renderer, texturaFondo, texturaTitulo, font);
                break;

            case CONFIGURACION:

                estadoActual = mostrarMenuConfiguracion(renderer, texturaFondo, &miConfig, font, texFlechaIzq, texFlechaDer);

                break;

            case JUGAR:

                solicitarNombreJugador(renderer, texturaFondo, &j1, font, 1);

                if (miConfig.cantJugadores == 2) {
                    solicitarNombreJugador(renderer, texturaFondo, &j2, font, 2);
                }

                estadoActual = jugarPartida(renderer, texturaFondo, &miConfig, &j1, &j2, font);

                estadoActual = MENU;
                break;

            case ESTADISTICAS:
                // logica de estadisticas

                estadoActual = MENU;
                break;

            case SALIR:

                corriendo = 0;
                break;
        }
    }


    SDL_DestroyTexture(texturaFondo);
    SDL_DestroyTexture(texturaTitulo);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
    TTF_CloseFont(font);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}


int mostrarMenuInicial(SDL_Renderer *renderer, SDL_Texture *texturaFondo, SDL_Texture *texturaTitulo,TTF_Font *font) {
    int modo = -1;
    int corriendo = 1;
    SDL_Event e;
    int mX, mY; // mouse


    SDL_Rect rectTitulo = { 150, 20, 500, 180 };
    SDL_Rect btnJugar = {250, 240, 300, 55};
    SDL_Rect btnEstadisticas = {250, 320, 300, 55};
    SDL_Rect btnSalir = {720, 520, 50, 50};

    SDL_Color colTierra = { 180, 90, 40, 255 };
    SDL_Color colTierraH = { 220, 120, 60, 255 };
    SDL_Color colTexto = { 255, 230, 170, 255 };
    SDL_Color colRojo = { 150, 40, 30, 255 };
    SDL_Color colRojoH = { 200, 50, 40, 255 };

    while(corriendo) {
        SDL_GetMouseState(&mX, &mY);
        SDL_Point p = {mX, mY};

        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT)
            {
                modo = SALIR;
                corriendo = 0;
            }
            if(e.type == SDL_MOUSEBUTTONDOWN) {
                if (SDL_PointInRect(&p, &btnJugar))
                {
                    modo = CONFIGURACION;
                    corriendo = 0;
                }
                else if (SDL_PointInRect(&p, &btnEstadisticas))
                {
                    modo = ESTADISTICAS;
                    corriendo = 0;
                }
                else if (SDL_PointInRect(&p, &btnSalir))
                {
                    modo = SALIR;
                    corriendo = 0;
                }
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texturaFondo, NULL, NULL);
        SDL_RenderCopy(renderer, texturaTitulo, NULL, &rectTitulo);


        SDL_SetRenderDrawColor(renderer, 45, 25, 10, 180);
        SDL_Rect sJ = {btnJugar.x+5, btnJugar.y+5, btnJugar.w, btnJugar.h};
        SDL_Rect sE = {btnEstadisticas.x+5, btnEstadisticas.y+5, btnEstadisticas.w, btnEstadisticas.h};
        SDL_RenderFillRect(renderer, &sJ);
        SDL_RenderFillRect(renderer, &sE);


        SDL_SetRenderDrawColor(renderer, SDL_PointInRect(&p, &btnJugar) ? colTierraH.r : colTierra.r,
                                          SDL_PointInRect(&p, &btnJugar) ? colTierraH.g : colTierra.g,
                                          SDL_PointInRect(&p, &btnJugar) ? colTierraH.b : colTierra.b, 255);
        SDL_RenderFillRect(renderer, &btnJugar);

        SDL_SetRenderDrawColor(renderer, SDL_PointInRect(&p, &btnEstadisticas) ? colTierraH.r : colTierra.r,
                                          SDL_PointInRect(&p, &btnEstadisticas) ? colTierraH.g : colTierra.g,
                                          SDL_PointInRect(&p, &btnEstadisticas) ? colTierraH.b : colTierra.b, 255);
        SDL_RenderFillRect(renderer, &btnEstadisticas);

        SDL_SetRenderDrawColor(renderer, SDL_PointInRect(&p, &btnSalir) ? colRojoH.r : colRojo.r,
                                          SDL_PointInRect(&p, &btnSalir) ? colRojoH.g : colRojo.g,
                                          SDL_PointInRect(&p, &btnSalir) ? colRojoH.b : colRojo.b, 255);
        SDL_RenderFillRect(renderer, &btnSalir);

        mostrarTexto(renderer, "JUGAR", font, btnJugar.x + 95, btnJugar.y + 8, colTexto);
        mostrarTexto(renderer, "ESTADISTICAS", font, btnEstadisticas.x + 35, btnEstadisticas.y + 8, colTexto);
        mostrarTexto(renderer, "X", font, btnSalir.x + 15, btnSalir.y + 5, colTexto);

        SDL_RenderPresent(renderer);
    }
    return modo;
}


