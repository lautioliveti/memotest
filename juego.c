#include "juego.h"
#include <stdlib.h>
#include <stdio.h>

#define MARGEN_SUPERIOR 100
#define MARGEN_IZQUIERDO 50
#define ANCHO_CARTA 100
#define ALTO_CARTA 100
#define ESPACIADO 10
#define DELAY_MOSTRAR_CARTAS 1000  // 1 segundo

// Crea una nueva instancia del juego
EstadoJuego* crearJuego(Configuracion *config) {
    EstadoJuego *juego = (EstadoJuego*)malloc(sizeof(EstadoJuego));
    if (!juego) {
        return NULL;
    }
    
    // Obtener dimensiones
    obtenerDimensionesTablero(config->dimension, &juego->filas, &juego->columnas);
    juego->totalCartas = juego->filas * juego->columnas;
    juego->totalPares = juego->totalCartas / 2;
    
    // Crear vector dinámico para el tablero
    juego->tablero = vectorCrear(juego->totalCartas, sizeof(Carta));
    if (!juego->tablero) {
        free(juego);
        return NULL;
    }
    
    // Inicializar estado
    juego->paresEncontrados = 0;
    juego->primeraCarta = -1;
    juego->segundaCarta = -1;
    juego->racha = 0;
    juego->esperando = 0;
    juego->tiempoEspera = 0;
    juego->setActual = config->setFiguras;
    
    // Inicializar tablero
    inicializarTablero(juego, config);
    
    return juego;
}

// Inicializa el tablero con cartas aleatorias
void inicializarTablero(EstadoJuego *juego, Configuracion *config) {
    int i, j;
    
    // Crear pares de cartas
    for (i = 0; i < juego->totalPares; i++) {
        // Puntos aleatorios entre 10 y 50
        int puntos = 10 + (rand() % 41);
        
        // Crear primera carta del par
        Carta carta1;
        carta1.idFigura = i;
        carta1.puntos = puntos;
        carta1.estado = ESTADO_OCULTA;
        vectorAgregar(juego->tablero, &carta1);
        
        // Crear segunda carta del par (misma figura y puntos)
        Carta carta2;
        carta2.idFigura = i;
        carta2.puntos = puntos;
        carta2.estado = ESTADO_OCULTA;
        vectorAgregar(juego->tablero, &carta2);
    }
    
    // Mezclar cartas (Fisher-Yates)
    vectorMezclar(juego->tablero);
    
    // Calcular posiciones en pantalla
    int indice = 0;
    for (i = 0; i < juego->filas; i++) {
        for (j = 0; j < juego->columnas; j++) {
            Carta *carta = (Carta*)vectorObtener(juego->tablero, indice);
            if (carta) {
                carta->rect.x = MARGEN_IZQUIERDO + j * (ANCHO_CARTA + ESPACIADO);
                carta->rect.y = MARGEN_SUPERIOR + i * (ALTO_CARTA + ESPACIADO);
                carta->rect.w = ANCHO_CARTA;
                carta->rect.h = ALTO_CARTA;
            }
            indice++;
        }
    }
}

// Obtiene el índice de la carta en la posición x,y
int obtenerCartaEnPosicion(EstadoJuego *juego, int x, int y) {
    SDL_Point punto = {x, y};
    
    for (size_t i = 0; i < vectorCantidad(juego->tablero); i++) {
        Carta *carta = (Carta*)vectorObtener(juego->tablero, i);
        if (carta && SDL_PointInRect(&punto, &carta->rect)) {
            return i;
        }
    }
    
    return -1;
}

// Calcula puntos con bonus por racha
static int calcularPuntos(int puntosBase, int racha) {
    float multiplicador = 1.0f + (racha * 0.1f);
    return (int)(puntosBase * multiplicador);
}

// Maneja el click en una carta
int manejarClickCarta(EstadoJuego *juego, DatosJugadores *jugadores, int x, int y) {
    // Si estamos esperando, ignorar clicks
    if (juego->esperando) {
        return 0;
    }
    
    int indiceCarta = obtenerCartaEnPosicion(juego, x, y);
    if (indiceCarta == -1) {
        return 0;
    }
    
    Carta *carta = (Carta*)vectorObtener(juego->tablero, indiceCarta);
    if (!carta) {
        return 0;
    }
    
    // Ignorar si ya está encontrada o revelada
    if (carta->estado == ESTADO_ENCONTRADA || carta->estado == ESTADO_REVELADA) {
        return 0;
    }
    
    // Primera carta
    if (juego->primeraCarta == -1) {
        juego->primeraCarta = indiceCarta;
        carta->estado = ESTADO_REVELADA;
        return 1;
    }
    
    // Segunda carta (no puede ser la misma que la primera)
    if (juego->segundaCarta == -1 && indiceCarta != juego->primeraCarta) {
        juego->segundaCarta = indiceCarta;
        carta->estado = ESTADO_REVELADA;
        juego->esperando = 1;
        juego->tiempoEspera = SDL_GetTicks() + DELAY_MOSTRAR_CARTAS;
        return 1;
    }
    
    return 0;
}

// Actualiza el estado del juego
void actualizarJuego(EstadoJuego *juego, DatosJugadores *jugadores) {
    if (!juego->esperando) {
        return;
    }
    
    // Verificar si pasó el tiempo de espera
    if (SDL_GetTicks() < juego->tiempoEspera) {
        return;
    }
    
    // Ya pasó el tiempo, verificar si las cartas coinciden
    Carta *carta1 = (Carta*)vectorObtener(juego->tablero, juego->primeraCarta);
    Carta *carta2 = (Carta*)vectorObtener(juego->tablero, juego->segundaCarta);
    
    if (!carta1 || !carta2) {
        juego->esperando = 0;
        juego->primeraCarta = -1;
        juego->segundaCarta = -1;
        return;
    }
    
    // Verificar si coinciden
    if (carta1->idFigura == carta2->idFigura) {
        // ¡Acierto!
        carta1->estado = ESTADO_ENCONTRADA;
        carta2->estado = ESTADO_ENCONTRADA;
        juego->paresEncontrados++;
        
        // Calcular y sumar puntos con bonus por racha
        int puntosGanados = calcularPuntos(carta1->puntos, juego->racha);
        jugadores->puntajes[jugadores->turnoActual] += puntosGanados;
        
        // Incrementar racha
        juego->racha++;
        
        printf("¡Acierto! +%d puntos (racha x%d)\n", puntosGanados, juego->racha);
        
        // En modo 2 jugadores, el jugador actual mantiene el turno
    } else {
        // Fallo - ocultar cartas
        carta1->estado = ESTADO_OCULTA;
        carta2->estado = ESTADO_OCULTA;
        
        // Resetear racha
        juego->racha = 0;
        
        // En modo 2 jugadores, cambiar turno
        if (jugadores && jugadores->turnoActual == 0) {
            jugadores->turnoActual = 1;
        } else if (jugadores) {
            jugadores->turnoActual = 0;
        }
        
        printf("Fallo - Racha reiniciada\n");
    }
    
    // Resetear selección
    juego->primeraCarta = -1;
    juego->segundaCarta = -1;
    juego->esperando = 0;
}

// Verifica si el juego terminó
int juegoTerminado(EstadoJuego *juego) {
    return juego->paresEncontrados >= juego->totalPares;
}

// Destruye el juego y libera memoria
void destruirJuego(EstadoJuego *juego) {
    if (juego) {
        if (juego->tablero) {
            vectorDestruir(juego->tablero);
        }
        free(juego);
    }
}
