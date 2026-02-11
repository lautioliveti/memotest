#include "configuracion.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "dibujo.h"

void solicitarNombreJugador(SDL_Renderer *renderer, SDL_Texture *texturaFondo, Jugador* jugador,TTF_Font *font,int numJugador) {
    char nombreTemp[TAM_NOMBRE] = "";
    SDL_StartTextInput();




    SDL_Color colorTierra = { 255, 120, 60, 255 };
    SDL_Color colorTierraH = { 255, 150, 90, 255 };
    SDL_Color colorBlanco = {255, 255, 255, 255};

    SDL_Event e;
    int corriendo = 1;
    int cursor = 0;
    int mX, mY;


    SDL_Rect btnSiguiente = {300, 400, 200, 50};

    while (corriendo) {
        SDL_GetMouseState(&mX, &mY);
        SDL_Point p = {mX, mY};

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                corriendo = 0;
            }

            if (e.type == SDL_TEXTINPUT) {
                if (strlen(nombreTemp) + strlen(e.text.text) < TAM_NOMBRE) {
                    strcat(nombreTemp, e.text.text);
                    cursor = strlen(nombreTemp);
                }
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE && cursor > 0) {
                    nombreTemp[--cursor] = '\0';
                } else if (e.key.keysym.sym == SDLK_RETURN && cursor > 0) {
                    corriendo = 0;
                }
            }


            if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (SDL_PointInRect(&p, &btnSiguiente) && cursor > 0) {
                    corriendo = 0;
                }
            }
        }


        SDL_RenderClear(renderer);


        SDL_RenderCopy(renderer, texturaFondo, NULL, NULL);

        // overlay
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
        SDL_Rect overlay = {150, 160, 500, 320};
        SDL_RenderFillRect(renderer, &overlay);

        char etiquetaJugador[20];
        sprintf(etiquetaJugador, "Jugador %d", numJugador);
        mostrarTexto(renderer, etiquetaJugador, font, 320, 180, colorTierra);
        mostrarTexto(renderer, "Ingresar nombre:", font, 250, 230, colorBlanco);


        if (strlen(nombreTemp) > 0) {
            mostrarTexto(renderer, nombreTemp, font, 370, 300, colorTierra);
        } else {
            mostrarTexto(renderer, "...", font, 370, 300, colorTierra);
        }


        if (SDL_PointInRect(&p, &btnSiguiente)) {
            SDL_SetRenderDrawColor(renderer, colorTierraH.r, colorTierraH.g, colorTierraH.b, 255);
        }
        else {
            SDL_SetRenderDrawColor(renderer, colorTierra.r, colorTierra.g, colorTierra.b, 255);
        }

        SDL_RenderFillRect(renderer, &btnSiguiente);
        mostrarTexto(renderer, "Aceptar", font, btnSiguiente.x + 35, btnSiguiente.y + 8, colorBlanco);

        SDL_RenderPresent(renderer);
    }

    strncpy(jugador->nombre, nombreTemp, TAM_NOMBRE);
    jugador->puntaje = 0;

    SDL_StopTextInput();
}

int mostrarMenuConfiguracion(SDL_Renderer* renderer, SDL_Texture* texturaFondo, Configuracion *config, TTF_Font *font, SDL_Texture* tIzq, SDL_Texture* tDer) {
    if (!font) return -1;

    SDL_Event e;
    int corriendo = 1;
    int estadoSiguiente = MENU;

    SDL_Rect fIzqDim = {500, 180,50,80},
    fDerDim = {640, 180, 50,80};

    SDL_Rect fIzqSet = {500, 260, 50,80},
    fDerSet = {640, 260, 50,80};

    SDL_Rect fIzqJug = {500, 340, 50,80},
    fDerJug = {640, 340, 50,80};

    SDL_Rect btnGuardar = {420, 480, 200, 60};
    SDL_Rect btnVolver = {180, 480, 200, 60};

    SDL_Color blanco = {255, 255, 255, 255};
    SDL_Color naranja = {255, 120, 60, 255};
    SDL_Color colRojo = { 150, 40, 30, 255 }, colRojoH = { 200, 50, 40, 255 };

    while (corriendo) {
        int mX, mY;
        SDL_GetMouseState(&mX, &mY);
        SDL_Point p = {mX, mY};

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
            {
                corriendo = 0;
                estadoSiguiente = SALIR;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {

                if (SDL_PointInRect(&p, &fDerDim) || SDL_PointInRect(&p, &fIzqDim)) {
                    if (config->filas == 3)
                    {
                        config->filas = 4;
                        config->columnas = 4;
                    }
                    else if (config->filas == 4 && config->columnas == 4)
                    {
                        config->filas = 4;
                        config->columnas = 5;
                    }
                    else
                    {
                        config->filas = 3;
                        config->columnas = 4;
                    }
                }
                if (SDL_PointInRect(&p, &fDerSet) || SDL_PointInRect(&p, &fIzqSet))
                {
                    config->setDeCartas = !config->setDeCartas;
                }
                if (SDL_PointInRect(&p, &fDerJug) || SDL_PointInRect(&p, &fIzqJug))
                {
                    config->cantJugadores = (config->cantJugadores == 1) ? 2 : 1;
                }
                if (SDL_PointInRect(&p, &btnGuardar))
                {
                    guardarConfiguracion(config); // GUARDO CFG EN .DAT
                    estadoSiguiente = JUGAR;
                    corriendo = 0;
                }
                if (SDL_PointInRect(&p, &btnVolver))
                {
                    estadoSiguiente = MENU;
                    corriendo = 0;
                }
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texturaFondo, NULL, NULL);

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
        SDL_Rect overlay = {100, 50, 600, 520};
        SDL_RenderFillRect(renderer, &overlay);

        mostrarTexto(renderer, "CONFIGURACION", font, 280, 80, naranja);



        // seccion dimension
        mostrarTexto(renderer, "Dimensiones:", font, 150, 200, blanco);
        char dimTxt[10]; sprintf(dimTxt, "%dx%d", config->filas, config->columnas);
        mostrarTexto(renderer, dimTxt, font, 570, 200, blanco);
        SDL_RenderCopy(renderer, tIzq, NULL, &fIzqDim);
        SDL_RenderCopy(renderer, tDer, NULL, &fDerDim);

        // seccion cartas
        mostrarTexto(renderer, "Set de cartas:", font, 150, 280, blanco);
        mostrarTexto(renderer, (config->setDeCartas == SET_A ? "SET A" : "SET B"), font, 550, 280, blanco);
        SDL_RenderCopy(renderer, tIzq, NULL, &fIzqSet);
        SDL_RenderCopy(renderer, tDer, NULL, &fDerSet);

        // seccion cant jugadores
        mostrarTexto(renderer, "Jugadores:", font, 150, 360, blanco);
        char jugTxt[5]; sprintf(jugTxt, "%d", config->cantJugadores);
        mostrarTexto(renderer, jugTxt, font, 585, 360, blanco);
        SDL_RenderCopy(renderer, tIzq, NULL, &fIzqJug);
        SDL_RenderCopy(renderer, tDer, NULL, &fDerJug);

        // boton volver y guardar
        SDL_SetRenderDrawColor(renderer, 0, SDL_PointInRect(&p, &btnGuardar) ? 200 : 130, 0, 255);
        SDL_RenderFillRect(renderer, &btnGuardar);
        mostrarTexto(renderer, "Guardar", font, btnGuardar.x + 35, btnGuardar.y + 12, blanco);

        if (SDL_PointInRect(&p, &btnVolver))
        {
            SDL_SetRenderDrawColor(renderer, colRojoH.r, colRojoH.g, colRojoH.b, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, colRojo.r, colRojo.g, colRojo.b, 255);

        }
        SDL_RenderFillRect(renderer, &btnVolver);
        mostrarTexto(renderer, "Volver", font, btnVolver.x + 35, btnVolver.y + 12, blanco);

        SDL_RenderPresent(renderer);
    }
    return estadoSiguiente;
}

void guardarConfiguracion(Configuracion *config) {
    FILE *arch = fopen("config.dat", "wb");
    if (arch)
    {
        fwrite(config, sizeof(Configuracion), 1, arch);
        fclose(arch);
    }
}

void cargarConfiguracion(Configuracion *config) {
    FILE *arch = fopen("config.dat", "rb");
    if (arch) {
        fread(config, sizeof(Configuracion), 1, arch);
        fclose(arch);
    }
    else
    {
        config->filas = 3;
        config->columnas = 4;
        config->setDeCartas = SET_A;
        config->cantJugadores = 1;
    }
}
