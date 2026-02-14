#include "modelo.h"

#include "vector.h"
#include <stdlib.h>


void intercambiarFiguras(Figura* a, Figura* b)
{
    Figura tmp = *a;
    *a = *b;
    *b = tmp;
}

void mezclarRandom(Vector* v)
{
    int n = tamVector(v);
    for (int i = n - 1; i > 0; --i)
    {
        int j = rand() % (i + 1);
        Figura* ci = (Figura*)obtenerVector(v, i);
        Figura* cj = (Figura*)obtenerVector(v, j);
        intercambiarFiguras(ci, cj);
    }
}

int inicializarTablero(Tablero* b, int filas, int columnas)
{
    b->filas = filas;
    b->columnas = columnas;
    b->numPares = (filas * columnas) / 2;
    b->primerIndice = -1;
    b->segundoIndice = -1;
    b->ocultar = 0;
    b->hideAtMs = 0;
    b->racha = 0;

    if (!crearVector(&b->figuras, sizeof(Figura), CAP_INICIAL))
        return -1;

    // Sistema de puntuacion mejorado:
    // Los puntos totales siempre suman lo mismo (300 puntos base)
    // Se distribuyen de forma aleatoria pero la suma es constante

    int PUNTOS_TOTALES = 300;
    int* puntosArray = (int*)malloc(sizeof(int) * b->numPares);

    if (!puntosArray) {
        destruirVector(&b->figuras);
        return -1;
    }

    // Distribuir puntos de forma más equitativa
    // Minimo 10 puntos por par, el resto se distribuye aleatoriamente
    int puntosMinimos = 10 * b->numPares;
    int puntosRestantes = PUNTOS_TOTALES - puntosMinimos;

    // Inicializar con el minimo
    for (int i = 0; i < b->numPares; i++) {
        puntosArray[i] = 10;
    }

    // Distribuir los puntos restantes aleatoriamente
    for (int i = 0; i < puntosRestantes; i++) {
        int idx = rand() % b->numPares;
        puntosArray[idx]++;
    }

    // Mezclar el array de puntos
    for (int i = b->numPares - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = puntosArray[i];
        puntosArray[i] = puntosArray[j];
        puntosArray[j] = temp;
    }

    // Crear las figuras con los puntos asignados
    for (int pid = 0; pid < b->numPares; pid++)
    {
        int puntos = puntosArray[pid];
        Figura c1 = { pid, puntos, ESCONDIDA };
        Figura c2 = { pid, puntos, ESCONDIDA };
        insertarFinalVector(&b->figuras, &c1);
        insertarFinalVector(&b->figuras, &c2);
    }

    free(puntosArray);
    mezclarRandom(&b->figuras);
    return 0;
}

void destruirTablero(Tablero* b)
{
    destruirVector(&b->figuras);
}
