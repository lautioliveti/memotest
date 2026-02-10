#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <stddef.h>

// TDA Vector genérico con memoria dinámica
typedef struct {
    void *datos;           // Puntero a los datos
    size_t capacidad;      // Capacidad total del vector
    size_t cantidad;       // Cantidad de elementos actuales
    size_t tamElemento;    // Tamaño de cada elemento
} Vector;

// Funciones del TDA Vector
Vector* vectorCrear(size_t capacidadInicial, size_t tamElemento);
int vectorAgregar(Vector *vec, const void *elemento);
void* vectorObtener(Vector *vec, size_t indice);
int vectorEstablecerEnIndice(Vector *vec, size_t indice, const void *elemento);
size_t vectorCantidad(const Vector *vec);
size_t vectorCapacidad(const Vector *vec);
void vectorMezclar(Vector *vec);
void vectorVaciar(Vector *vec);
void vectorDestruir(Vector *vec);

#endif // VECTOR_H_INCLUDED
