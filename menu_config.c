#include "configuracion.h"
#include "dibujo.h"
#include "menu_config.h"
#include <string.h>
#include <stdio.h>

// Función auxiliar para mostrar un botón con texto
static void dibujarBoton(SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color, SDL_Color colorHover,
                         int hover, char *texto, TTF_Font *font, SDL_Color colorTexto) {
    // Sombra
    SDL_SetRenderDrawColor(renderer, 45, 25, 10, 180);
    SDL_Rect sombra = {rect.x + 5, rect.y + 5, rect.w, rect.h};
    SDL_RenderFillRect(renderer, &sombra);

    // Botón con efecto hover
    if (hover) {
        SDL_SetRenderDrawColor(renderer, colorHover.r, colorHover.g, colorHover.b, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    }
    SDL_RenderFillRect(renderer, &rect);

    // Borde
    SDL_SetRenderDrawColor(renderer, 80, 50, 20, 255);
    SDL_RenderDrawRect(renderer, &rect);

    // Texto centrado
    SDL_Surface *surf = TTF_RenderText_Blended(font, texto, colorTexto);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
    int texW = surf->w;
    int texH = surf->h;
    SDL_Rect rectTexto = {rect.x + (rect.w - texW) / 2, rect.y + (rect.h - texH) / 2, texW, texH};
    SDL_RenderCopy(renderer, tex, NULL, &rectTexto);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}

// Muestra el menú de configuración
int mostrarMenuConfiguracion(SDL_Renderer *renderer, SDL_Texture *texturaFondo, Configuracion *config) {
    int corriendo = 1;
    int confirmado = 0;
    SDL_Event e;
    int mX, mY;

    TTF_Font *fontTitulo = TTF_OpenFont("fnt/BitcountGridSingle_Cursive-Regular.ttf", 36);
    TTF_Font *font = TTF_OpenFont("fnt/BitcountGridSingle_Cursive-Regular.ttf", 24);
    TTF_Font *fontPeq = TTF_OpenFont("fnt/BitcountGridSingle_Cursive-Regular.ttf", 20);

    if (!fontTitulo || !font || !fontPeq) {
        printf("Error al cargar fuentes\n");
        return 0;
    }

    // Cargar configuración previa
    cargarConfiguracion(config);

    // Definir rectángulos de botones
    SDL_Rect btn3x4 = {100, 180, 180, 45};
    SDL_Rect btn4x4 = {310, 180, 180, 45};
    SDL_Rect btn4x5 = {520, 180, 180, 45};

    SDL_Rect btn1Jugador = {200, 280, 180, 45};
    SDL_Rect btn2Jugadores = {420, 280, 180, 45};

    SDL_Rect btnSetPack1 = {200, 380, 180, 45};
    SDL_Rect btnSetPack2 = {420, 380, 180, 45};

    SDL_Rect btnAceptar = {250, 480, 150, 50};
    SDL_Rect btnCancelar = {420, 480, 150, 50};

    SDL_Color colTierra = {180, 90, 40, 255};
    SDL_Color colTierraH = {220, 120, 60, 255};
    SDL_Color colVerde = {60, 120, 60, 255};
    SDL_Color colVerdeH = {80, 160, 80, 255};
    SDL_Color colRojo = {150, 40, 30, 255};
    SDL_Color colRojoH = {200, 50, 40, 255};
    SDL_Color colTexto = {255, 230, 170, 255};
    SDL_Color colBlanco = {255, 255, 255, 255};
    SDL_Color colAmarillo = {255, 220, 100, 255};

    while(corriendo) {
        SDL_GetMouseState(&mX, &mY);
        SDL_Point p = {mX, mY};

        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                corriendo = 0;
                confirmado = 0;
            }
            if(e.type == SDL_MOUSEBUTTONDOWN) {
                // Dimensiones
                if (SDL_PointInRect(&p, &btn3x4)) {
                    config->dimension = DIM_3X4;
                }
                else if (SDL_PointInRect(&p, &btn4x4)) {
                    config->dimension = DIM_4X4;
                }
                else if (SDL_PointInRect(&p, &btn4x5)) {
                    config->dimension = DIM_4X5;
                }
                // Cantidad de jugadores
                else if (SDL_PointInRect(&p, &btn1Jugador)) {
                    config->cantidadJugadores = 1;
                }
                else if (SDL_PointInRect(&p, &btn2Jugadores)) {
                    config->cantidadJugadores = 2;
                }
                // Set de figuras
                else if (SDL_PointInRect(&p, &btnSetPack1)) {
                    config->setFiguras = SET_PACK1;
                }
                else if (SDL_PointInRect(&p, &btnSetPack2)) {
                    config->setFiguras = SET_PACK2;
                }
                // Aceptar/Cancelar
                else if (SDL_PointInRect(&p, &btnAceptar)) {
                    guardarConfiguracion(config);
                    corriendo = 0;
                    confirmado = 1;
                }
                else if (SDL_PointInRect(&p, &btnCancelar)) {
                    corriendo = 0;
                    confirmado = 0;
                }
            }
        }

        // Renderizado
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texturaFondo, NULL, NULL);

        // Título
        mostrarTexto(renderer, "CONFIGURACION", fontTitulo, 230, 30, colAmarillo);

        // Labels
        mostrarTexto(renderer, "Dimension del tablero:", font, 100, 140, colBlanco);
        mostrarTexto(renderer, "Cantidad de jugadores:", font, 200, 240, colBlanco);
        mostrarTexto(renderer, "Set de figuras:", font, 200, 340, colBlanco);

        // Botones de dimensión
        dibujarBoton(renderer, btn3x4,
                    config->dimension == DIM_3X4 ? colVerdeH : colTierra,
                    colTierraH,
                    SDL_PointInRect(&p, &btn3x4),
                    "3x4 (6)", fontPeq, colTexto);

        dibujarBoton(renderer, btn4x4,
                    config->dimension == DIM_4X4 ? colVerdeH : colTierra,
                    colTierraH,
                    SDL_PointInRect(&p, &btn4x4),
                    "4x4 (8)", fontPeq, colTexto);

        dibujarBoton(renderer, btn4x5,
                    config->dimension == DIM_4X5 ? colVerdeH : colTierra,
                    colTierraH,
                    SDL_PointInRect(&p, &btn4x5),
                    "4x5 (10)", fontPeq, colTexto);

        // Botones de jugadores
        dibujarBoton(renderer, btn1Jugador,
                    config->cantidadJugadores == 1 ? colVerdeH : colTierra,
                    colTierraH,
                    SDL_PointInRect(&p, &btn1Jugador),
                    "1 Jugador", fontPeq, colTexto);

        dibujarBoton(renderer, btn2Jugadores,
                    config->cantidadJugadores == 2 ? colVerdeH : colTierra,
                    colTierraH,
                    SDL_PointInRect(&p, &btn2Jugadores),
                    "2 Jugadores", fontPeq, colTexto);

        // Botones de sets
        dibujarBoton(renderer, btnSetPack1,
                    config->setFiguras == SET_PACK1 ? colVerdeH : colTierra,
                    colTierraH,
                    SDL_PointInRect(&p, &btnSetPack1),
                    "Pack 1", fontPeq, colTexto);

        dibujarBoton(renderer, btnSetPack2,
                    config->setFiguras == SET_PACK2 ? colVerdeH : colTierra,
                    colTierraH,
                    SDL_PointInRect(&p, &btnSetPack2),
                    "Pack 2", fontPeq, colTexto);

        // Botones Aceptar/Cancelar
        dibujarBoton(renderer, btnAceptar, colVerde, colVerdeH,
                    SDL_PointInRect(&p, &btnAceptar),
                    "Aceptar", font, colBlanco);

        dibujarBoton(renderer, btnCancelar, colRojo, colRojoH,
                    SDL_PointInRect(&p, &btnCancelar),
                    "Cancelar", font, colBlanco);

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(fontTitulo);
    TTF_CloseFont(font);
    TTF_CloseFont(fontPeq);

    return confirmado;
}

// Solicita los nombres de los jugadores
int solicitarNombresJugadores(SDL_Renderer *renderer, SDL_Texture *texturaFondo,
                               DatosJugadores *jugadores, int cantJugadores) {
    int jugadorActual = 0;
    char nombreTemporal[MAX_NOMBRE + 1] = "";
    int corriendo = 1;
    int confirmado = 0;
    SDL_Event e;
    int mX, mY;

    TTF_Font *fontTitulo = TTF_OpenFont("fnt/BitcountGridSingle_Cursive-Regular.ttf", 36);
    TTF_Font *font = TTF_OpenFont("fnt/BitcountGridSingle_Cursive-Regular.ttf", 28);

    if (!fontTitulo || !font) {
        printf("Error al cargar fuentes\n");
        return 0;
    }

    SDL_Rect btnContinuar = {325, 400, 150, 50};
    SDL_Rect rectInput = {200, 280, 400, 50};

    SDL_Color colVerde = {60, 120, 60, 255};
    SDL_Color colVerdeH = {80, 160, 80, 255};
    SDL_Color colTexto = {255, 230, 170, 255};
    SDL_Color colBlanco = {255, 255, 255, 255};
    SDL_Color colAmarillo = {255, 220, 100, 255};

    // Inicializar nombres vacíos
    for (int i = 0; i < MAX_JUGADORES; i++) {
        jugadores->nombres[i][0] = '\0';
        jugadores->puntajes[i] = 0;
    }
    jugadores->turnoActual = 0;

    SDL_StartTextInput();

    while(corriendo) {
        SDL_GetMouseState(&mX, &mY);
        SDL_Point p = {mX, mY};

        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                corriendo = 0;
                confirmado = 0;
            }
            else if(e.type == SDL_TEXTINPUT) {
                // Solo agregar si no excede el límite
                if (strlen(nombreTemporal) < MAX_NOMBRE) {
                    strcat(nombreTemporal, e.text.text);
                }
            }
            else if(e.type == SDL_KEYDOWN) {
                if(e.key.keysym.sym == SDLK_BACKSPACE && strlen(nombreTemporal) > 0) {
                    nombreTemporal[strlen(nombreTemporal) - 1] = '\0';
                }
                else if(e.key.keysym.sym == SDLK_RETURN && strlen(nombreTemporal) > 0) {
                    // Guardar nombre y pasar al siguiente jugador
                    strcpy(jugadores->nombres[jugadorActual], nombreTemporal);
                    jugadorActual++;

                    if (jugadorActual >= cantJugadores) {
                        corriendo = 0;
                        confirmado = 1;
                    } else {
                        nombreTemporal[0] = '\0';
                    }
                }
            }
            else if(e.type == SDL_MOUSEBUTTONDOWN) {
                if (SDL_PointInRect(&p, &btnContinuar) && strlen(nombreTemporal) > 0) {
                    strcpy(jugadores->nombres[jugadorActual], nombreTemporal);
                    jugadorActual++;

                    if (jugadorActual >= cantJugadores) {
                        corriendo = 0;
                        confirmado = 1;
                    } else {
                        nombreTemporal[0] = '\0';
                    }
                }
            }
        }

        // Renderizado
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texturaFondo, NULL, NULL);

        // Título
        mostrarTexto(renderer, "DATOS DE JUGADORES", fontTitulo, 180, 80, colAmarillo);

        // Indicación del jugador
        char textoJugador[50];
        sprintf(textoJugador, "Jugador %d:", jugadorActual + 1);
        mostrarTexto(renderer, textoJugador, font, 200, 200, colBlanco);

        // Campo de entrada de texto
        SDL_SetRenderDrawColor(renderer, 45, 25, 10, 180);
        SDL_Rect sombra = {rectInput.x + 5, rectInput.y + 5, rectInput.w, rectInput.h};
        SDL_RenderFillRect(renderer, &sombra);

        SDL_SetRenderDrawColor(renderer, 240, 230, 210, 255);
        SDL_RenderFillRect(renderer, &rectInput);
        SDL_SetRenderDrawColor(renderer, 80, 50, 20, 255);
        SDL_RenderDrawRect(renderer, &rectInput);

        // Mostrar texto ingresado con cursor parpadeante
        if (strlen(nombreTemporal) > 0) {
            char textoConCursor[MAX_NOMBRE + 2];
            strcpy(textoConCursor, nombreTemporal);
            if ((SDL_GetTicks() / 500) % 2 == 0) {
                strcat(textoConCursor, "|");
            }
            SDL_Color colNegro = {20, 10, 5, 255};
            mostrarTexto(renderer, textoConCursor, font, rectInput.x + 10, rectInput.y + 10, colNegro);
        } else {
            // Cursor parpadeante cuando está vacío
            if ((SDL_GetTicks() / 500) % 2 == 0) {
                SDL_Color colGris = {100, 90, 80, 255};
                mostrarTexto(renderer, "|", font, rectInput.x + 10, rectInput.y + 10, colGris);
            }
        }

        // Instrucciones
        mostrarTexto(renderer, "Presiona ENTER o haz clic en Continuar", font, 150, 470, colTexto);

        // Botón Continuar
        if (strlen(nombreTemporal) > 0) {
            dibujarBoton(renderer, btnContinuar, colVerde, colVerdeH,
                        SDL_PointInRect(&p, &btnContinuar),
                        "Continuar", font, colBlanco);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_StopTextInput();
    TTF_CloseFont(fontTitulo);
    TTF_CloseFont(font);

    return confirmado;
}
