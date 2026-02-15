#include "estadisticas.h"
#include "dibujo.h"
#include <string.h>
#include <stdio.h>

int inicializarTopSesion(TopSesion* top) {
    if (!crearVector(&top->entradas, sizeof(EntradaTop), TOP_MAX)) {
        return -1;
    }
    top->cantidad = 0;
    return 0;
}

void destruirTopSesion(TopSesion* top) {
    if (top) {
        destruirVector(&top->entradas);
        top->cantidad = 0;
    }
}

// Funcion auxiliar para intercambiar dos entradas
static void intercambiarEntradas(EntradaTop* a, EntradaTop* b) {
    EntradaTop temp = *a;
    *a = *b;
    *b = temp;
}

void ordenarTop(TopSesion* top) {
    // Ordenamiento por burbuja descendente (mayor a menor puntaje)
    for (int i = 0; i < top->cantidad - 1; i++) {
        for (int j = 0; j < top->cantidad - i - 1; j++) {
            EntradaTop* e1 = (EntradaTop*)obtenerVector(&top->entradas, j);
            EntradaTop* e2 = (EntradaTop*)obtenerVector(&top->entradas, j + 1);

            if (e1->puntaje < e2->puntaje) {
                intercambiarEntradas(e1, e2);
            }
        }
    }
}

int agregarEntradaTop(TopSesion* top, const char* nombre, int puntaje) {
    if (!top || !nombre) return -1;

    // Crear nueva entrada
    EntradaTop nuevaEntrada;
    strncpy(nuevaEntrada.nombre, nombre, TAM_NOMBRE - 1);
    nuevaEntrada.nombre[TAM_NOMBRE - 1] = '\0';
    nuevaEntrada.puntaje = puntaje;

    // Si aún no llegamos al límite, simplemente agregamos
    if (top->cantidad < TOP_MAX) {
        if (insertarFinalVector(&top->entradas, &nuevaEntrada) != 0) {
            return -1;
        }
        top->cantidad++;
        ordenarTop(top);
        return 0;
    }

    // Si ya tenemos TOP_MAX entradas, verificar si el nuevo puntaje es mejor que el último
    EntradaTop* ultimaEntrada = (EntradaTop*)obtenerVector(&top->entradas, TOP_MAX - 1);

    if (puntaje > ultimaEntrada->puntaje) {
        // Reemplazar la última entrada
        *ultimaEntrada = nuevaEntrada;
        ordenarTop(top);
        return 0;
    }

    return 0; // No entra en el top pero no es error
}

int mostrarEstadisticas(SDL_Renderer* renderer, SDL_Texture* texturaFondo,
                        TopSesion* top, TTF_Font* font, TTF_Font* fontSmall) {
    if (!renderer || !font || !fontSmall) return -1;

    SDL_Event e;
    int corriendo = 1;
    int estadoSiguiente = MENU;

    SDL_Color blanco = {255, 255, 255, 255};
    SDL_Color dorado = {255, 215, 0, 255};
    SDL_Color plata = {192, 192, 192, 255};
    SDL_Color bronce = {205, 127, 50, 255};
    SDL_Color naranja = {255, 120, 60, 255};
    SDL_Color colorTexto = {230, 230, 230, 255};
    SDL_Color colRojo = {150, 40, 30, 255};
    SDL_Color colRojoH = {200, 50, 40, 255};

    SDL_Rect btnVolver = {300, 520, 200, 50};

    while (corriendo) {
        int mX, mY;
        SDL_GetMouseState(&mX, &mY);
        SDL_Point p = {mX, mY};

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                corriendo = 0;
                estadoSiguiente = SALIR;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (SDL_PointInRect(&p, &btnVolver)) {
                    corriendo = 0;
                    estadoSiguiente = MENU;
                }
            }

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                corriendo = 0;
                estadoSiguiente = MENU;
            }
        }

        // Renderizado
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texturaFondo, NULL, NULL);

        // Overlay semi-transparente
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
        SDL_Rect overlay = {80, 40, 640, 540};
        SDL_RenderFillRect(renderer, &overlay);

        // Título
        mostrarTexto(renderer, "TOP 10", font, 330, 60, naranja);

        // Línea decorativa
        SDL_SetRenderDrawColor(renderer, naranja.r, naranja.g, naranja.b, 255);
        SDL_Rect linea = {120, 110, 560, 3};
        SDL_RenderFillRect(renderer, &linea);

        // Mostrar entradas del Top
        if (top->cantidad == 0) {
            mostrarTexto(renderer, "No hay estadisticas.", fontSmall, 220, 290, colorTexto);
        } else {
            int yPos = 140;
            for (int i = 0; i < top->cantidad && i < TOP_MAX; i++) {
                EntradaTop* entrada = (EntradaTop*)obtenerVector(&top->entradas, i);

                if (!entrada) continue;

                // Determinar color según posición
                SDL_Color colorPosicion;
                if (i == 0) colorPosicion = dorado;
                else if (i == 1) colorPosicion = plata;
                else if (i == 2) colorPosicion = bronce;
                else colorPosicion = colorTexto;


                // Formato: posición - nombre - puntaje
                char textoEntrada[64];
                sprintf(textoEntrada, "%2d.", i + 1);
                mostrarTexto(renderer, textoEntrada, fontSmall, 200, yPos, colorPosicion);

                // Nombre del jugador
                sprintf(textoEntrada, "%s", entrada->nombre);
                mostrarTexto(renderer, textoEntrada, fontSmall, 260, yPos, blanco);

                // Puntaje alineado a la derecha
                sprintf(textoEntrada, "%d pts", entrada->puntaje);
                mostrarTexto(renderer, textoEntrada, fontSmall, 500, yPos, colorPosicion);

                yPos += 40;
            }
        }

        // Botón volver
        if (SDL_PointInRect(&p, &btnVolver)) {
            SDL_SetRenderDrawColor(renderer, colRojoH.r, colRojoH.g, colRojoH.b, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, colRojo.r, colRojo.g, colRojo.b, 255);
        }
        SDL_RenderFillRect(renderer, &btnVolver);
        mostrarTexto(renderer, "VOLVER", fontSmall, btnVolver.x + 45, btnVolver.y + 7, blanco);

        SDL_RenderPresent(renderer);
    }

    return estadoSiguiente;
}
