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

    if (!crearVector(&b->figuras, sizeof(Figura), CAP_INICIAL))
        return -1;

    for (int pid = 0; pid < b->numPares; pid++)
    {
        int puntos = 10 + (rand() % 91);
        Figura c1 = { pid, puntos, ESCONDIDA };
        Figura c2 = { pid, puntos, ESCONDIDA };
        insertarFinalVector(&b->figuras, &c1);
        insertarFinalVector(&b->figuras, &c2);
    }

    mezclarRandom(&b->figuras);
    return 0;
}

void destruirTablero(Tablero* b)
{
    destruirVector(&b->figuras);
}
