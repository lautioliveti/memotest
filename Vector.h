#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define CAP_INICIAL 4

typedef struct
{
    void* vec;
    int ce;
    size_t cap;
    size_t tamElem;
}Vector;

bool crearVector(Vector* v, size_t tamElem, size_t cap);
void destruirVector(Vector* v);
int tamVector(const Vector* v);
void* obtenerVector(Vector* v, int index);
int redimensionarVector(Vector* v, size_t capMinima);
int insertarFinalVector(Vector* v, const void* elem);


#endif // VECTOR_H_INCLUDED
