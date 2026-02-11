#include "juego.h"
#include "dibujo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Cargar texturas de cartas según el set elegido
int cargarTexturasCartas(SDL_Renderer *renderer, TexturasCartas *tc, int setDeCartas, int numPares) {
    tc->cantidad = numPares;
    tc->texturas = (SDL_Texture**)malloc(sizeof(SDL_Texture*) * numPares);

    if (!tc->texturas) return -1;

    // Cargar dorso de la carta
    tc->dorso = IMG_LoadTexture(renderer, "img/dorso.png");
    if (!tc->dorso) {
        printf("Error cargando dorso: %s\n", IMG_GetError());
        free(tc->texturas);
        return -1;
    }

    // Determinar carpeta según el set
    const char* carpeta = (setDeCartas == SET_A) ? "img/setA" : "img/setB";

    // Cargar cada textura de figura
    for (int i = 0; i < numPares; i++) {
        char ruta[256];
        sprintf(ruta, "%s/carta%d.png", carpeta, i);
        tc->texturas[i] = IMG_LoadTexture(renderer, ruta);

        if (!tc->texturas[i]) {
            printf("Error cargando %s: %s\n", ruta, IMG_GetError());
            // Liberar texturas ya cargadas
            for (int j = 0; j < i; j++) {
                SDL_DestroyTexture(tc->texturas[j]);
            }
            SDL_DestroyTexture(tc->dorso);
            free(tc->texturas);
            return -1;
        }
    }

    return 0;
}

void liberarTexturasCartas(TexturasCartas *tc) {
    if (tc->texturas) {
        for (int i = 0; i < tc->cantidad; i++) {
            if (tc->texturas[i]) {
                SDL_DestroyTexture(tc->texturas[i]);
            }
        }
        free(tc->texturas);
    }
    if (tc->dorso) {
        SDL_DestroyTexture(tc->dorso);
    }
}

void dibujarTablero(SDL_Renderer *renderer, Tablero *tablero,
                    TexturasCartas *tc, int offsetX, int offsetY,
                    int anchoC, int altoC, int mouseX, int mouseY) {

    for (int i = 0; i < tablero->filas; i++) {
        for (int j = 0; j < tablero->columnas; j++) {
            int idx = i * tablero->columnas + j;
            Figura *f = (Figura*)obtenerVector(&tablero->figuras, idx);

            SDL_Rect destRect = {
                offsetX + j * (anchoC + 10),
                offsetY + i * (altoC + 10),
                anchoC, altoC
            };

            // Efecto hover si el mouse está encima
            SDL_Point p = {mouseX, mouseY};
            int hover = SDL_PointInRect(&p, &destRect);

            if (f->estado == ESCONDIDA) {
                SDL_RenderCopy(renderer, tc->dorso, NULL, &destRect);

                // Efecto hover para cartas ocultas
                if (hover) {
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50);
                    SDL_RenderFillRect(renderer, &destRect);
                }
            } else if (f->estado == REVELADA || f->estado == ENCONTRADA) {
                SDL_RenderCopy(renderer, tc->texturas[f->parID], NULL, &destRect);

                // Efecto para cartas encontradas
                if (f->estado == ENCONTRADA) {
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 80);
                    SDL_RenderFillRect(renderer, &destRect);
                }
            }

            // Borde de la carta
            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            SDL_RenderDrawRect(renderer, &destRect);
        }
    }
}

void dibujarHUD(SDL_Renderer *renderer, TTF_Font *font, Jugador *j1, Jugador *j2,
                int jugadorActual, int cantJugadores, int paresEncontrados, int totalPares) {

    SDL_Color blanco = {255, 255, 255, 255};
    SDL_Color naranja = {255, 150, 60, 255};
    SDL_Color verde = {100, 255, 100, 255};

    // Fondo del HUD
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_Rect hudRect = {0, 0, 800, 60};
    SDL_RenderFillRect(renderer, &hudRect);

    // Información jugador 1
    char texto[100];
    sprintf(texto, "%s: %d pts", j1->nombre, j1->puntaje);
    mostrarTexto(renderer, texto, font, 20, 15,
                 (jugadorActual == 0) ? verde : blanco);

    // Información jugador 2 (si existe)
    if (cantJugadores == 2) {
        sprintf(texto, "%s: %d pts", j2->nombre, j2->puntaje);
        mostrarTexto(renderer, texto, font, 400, 15,
                     (jugadorActual == 1) ? verde : blanco);
    }

    // Progreso
    sprintf(texto, "Pares: %d/%d", paresEncontrados, totalPares);
    mostrarTexto(renderer, texto, font, 650, 15, naranja);
}

int obtenerCartaCliqueada(int mouseX, int mouseY, Tablero *tablero,
                          int offsetX, int offsetY, int anchoC, int altoC) {
    for (int i = 0; i < tablero->filas; i++) {
        for (int j = 0; j < tablero->columnas; j++) {
            SDL_Rect cartaRect = {
                offsetX + j * (anchoC + 10),
                offsetY + i * (altoC + 10),
                anchoC, altoC
            };

            SDL_Point p = {mouseX, mouseY};
            if (SDL_PointInRect(&p, &cartaRect)) {
                return i * tablero->columnas + j;
            }
        }
    }
    return -1;
}

void procesarSeleccion(Tablero *tablero, int indice, Jugador *jugadorActivo, int *jugadorActual, int cantJugadores) {
    Figura *f = (Figura*)obtenerVector(&tablero->figuras, indice);

    // No permitir seleccionar cartas ya encontradas o reveladas
    if (f->estado != ESCONDIDA) return;

    // Si ya hay un proceso de ocultamiento en curso, no permitir más selecciones
    if (tablero->ocultar) return;

    if (tablero->primerIndice == -1) {
        // Primera carta seleccionada
        tablero->primerIndice = indice;
        f->estado = REVELADA;
    } else if (tablero->segundoIndice == -1 && indice != tablero->primerIndice) {
        // Segunda carta seleccionada
        tablero->segundoIndice = indice;
        f->estado = REVELADA;

        // Verificar si son pareja
        Figura *f1 = (Figura*)obtenerVector(&tablero->figuras, tablero->primerIndice);
        Figura *f2 = (Figura*)obtenerVector(&tablero->figuras, tablero->segundoIndice);

        if (f1->parID == f2->parID) {
            // ¡Pareja encontrada!
            f1->estado = ENCONTRADA;
            f2->estado = ENCONTRADA;

            // Sumar puntos
            jugadorActivo->puntaje += f1->puntos;

            // Resetear para nueva selección
            tablero->primerIndice = -1;
            tablero->segundoIndice = -1;

            // En modo 1 jugador, el turno continúa
            // En modo 2 jugadores, el turno continúa si acertó
        } else {
            // No coinciden - programar para ocultar
            tablero->ocultar = 1;
            tablero->hideAtMs = SDL_GetTicks() + 1000; // 1 segundo para ver las cartas
        }
    }
}

void actualizarOcultamiento(Tablero *tablero) {
    if (tablero->ocultar && SDL_GetTicks() >= tablero->hideAtMs) {
        // Ocultar las cartas que no coincidieron
        Figura *f1 = (Figura*)obtenerVector(&tablero->figuras, tablero->primerIndice);
        Figura *f2 = (Figura*)obtenerVector(&tablero->figuras, tablero->segundoIndice);

        f1->estado = ESCONDIDA;
        f2->estado = ESCONDIDA;

        // Resetear
        tablero->primerIndice = -1;
        tablero->segundoIndice = -1;
        tablero->ocultar = 0;
    }
}

int contarParesEncontrados(Tablero *tablero) {
    int count = 0;
    int total = tablero->filas * tablero->columnas;

    for (int i = 0; i < total; i++) {
        Figura *f = (Figura*)obtenerVector(&tablero->figuras, i);
        if (f->estado == ENCONTRADA) {
            count++;
        }
    }

    return count / 2; // Cada par tiene 2 cartas
}

int jugarPartida(SDL_Renderer *renderer, SDL_Texture *texturaFondo,
                 Configuracion *cfg, Jugador *j1, Jugador *j2, TTF_Font *font) {

    // Inicializar tablero
    Tablero tablero;
    if (inicializarTablero(&tablero, cfg->filas, cfg->columnas) != 0) {
        printf("Error inicializando tablero\n");
        return MENU;
    }

    // Cargar texturas
    TexturasCartas tc;
    if (cargarTexturasCartas(renderer, &tc, cfg->setDeCartas, tablero.numPares) != 0) {
        printf("Error cargando texturas de cartas\n");
        destruirTablero(&tablero);
        return MENU;
    }

    // Configuración de visualización
    int anchoVentana = 800, altoVentana = 600;
    int anchoC = 100, altoC = 120;

    // Calcular offsets para centrar el tablero
    int anchoTotal = tablero.columnas * (anchoC + 10) - 10;
    int altoTotal = tablero.filas * (altoC + 10) - 10;
    int offsetX = (anchoVentana - anchoTotal) / 2;
    int offsetY = 80 + (altoVentana - 80 - altoTotal) / 2;

    // Variables de juego
    int corriendo = 1;
    int jugadorActual = 0;
    int estadoSiguiente = MENU;

    SDL_Event e;

    // Loop principal del juego
    while (corriendo) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                corriendo = 0;
                estadoSiguiente = SALIR;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int cartaIdx = obtenerCartaCliqueada(mouseX, mouseY, &tablero,
                                                     offsetX, offsetY, anchoC, altoC);

                if (cartaIdx != -1) {
                    Jugador *jugadorActivo = (jugadorActual == 0) ? j1 : j2;

                    int paresAntes = contarParesEncontrados(&tablero);
                    procesarSeleccion(&tablero, cartaIdx, jugadorActivo, &jugadorActual, cfg->cantJugadores);
                    int paresDespues = contarParesEncontrados(&tablero);

                    // Cambiar turno en modo 2 jugadores si falló
                    if (cfg->cantJugadores == 2 && tablero.ocultar && paresDespues == paresAntes) {
                        // Cambiar turno después de ocultar
                    }
                }
            }

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                corriendo = 0;
            }
        }

        // Actualizar lógica
        actualizarOcultamiento(&tablero);

        // Cambiar turno si falló (en modo 2 jugadores)
        static int ultimoEstadoOcultar = 0;

        if (cfg->cantJugadores == 2 && !tablero.ocultar && tablero.primerIndice == -1) {
            if (ultimoEstadoOcultar == 1) {
                jugadorActual = 1 - jugadorActual;
            }
            ultimoEstadoOcultar = 0;
        } else if (tablero.ocultar) {
            ultimoEstadoOcultar = 1;
        }

        // Verificar victoria
        int paresEncontrados = contarParesEncontrados(&tablero);
        if (paresEncontrados == tablero.numPares) {
            corriendo = 0;

            // Guardar estadísticas
        }

        // Renderizado
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texturaFondo, NULL, NULL);

        dibujarTablero(renderer, &tablero, &tc, offsetX, offsetY, anchoC, altoC, mouseX, mouseY);
        dibujarHUD(renderer, font, j1, j2, jugadorActual, cfg->cantJugadores, paresEncontrados, tablero.numPares);

        // Mostrar mensaje de victoria
        if (paresEncontrados == tablero.numPares) {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
            SDL_Rect overlayVictoria = {200, 200, 400, 200};
            SDL_RenderFillRect(renderer, &overlayVictoria);

            SDL_Color dorado = {255, 215, 0, 255};
            mostrarTexto(renderer, "VICTORIA", font, 320, 230, dorado);

            if (cfg->cantJugadores == 1) {
                char msg[100];
                sprintf(msg, "%s: %d puntos", j1->nombre, j1->puntaje);
                mostrarTexto(renderer, msg, font, 280, 290, dorado);
            } else {
                char msg[100];
                if (j1->puntaje > j2->puntaje) {
                    sprintf(msg, "¡Gana %s!", j1->nombre);
                } else if (j2->puntaje > j1->puntaje) {
                    sprintf(msg, "¡Gana %s!", j2->nombre);
                } else {
                    sprintf(msg, "¡Empate!");
                }
                mostrarTexto(renderer, msg, font, 300, 290, dorado);

            }

            SDL_Color blanco = {255, 255, 255, 255};
            SDL_RenderPresent(renderer);
            esperar(5000);

        }

        SDL_RenderPresent(renderer);
    }

    // Liberar recursos
    liberarTexturasCartas(&tc);
    destruirTablero(&tablero);

    return estadoSiguiente;
}
