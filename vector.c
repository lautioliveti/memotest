#include "vector.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Crea un nuevo vector con capacidad inicial
Vector* vectorCrear(size_t capacidadInicial, size_t tamElemento) {
    if (capacidadInicial == 0 || tamElemento == 0) {
        return NULL;
    }
    
    Vector *vec = (Vector*)malloc(sizeof(Vector));
    if (!vec) {
        return NULL;
    }
    
    vec->datos = malloc(capacidadInicial * tamElemento);
    if (!vec->datos) {
        free(vec);
        return NULL;
    }
    
    vec->capacidad = capacidadInicial;
    vec->cantidad = 0;
    vec->tamElemento = tamElemento;
    
    return vec;
}

// Redimensiona el vector si es necesario
static int vectorRedimensionar(Vector *vec, size_t nuevaCapacidad) {
    void *nuevosDatos = realloc(vec->datos, nuevaCapacidad * vec->tamElemento);
    if (!nuevosDatos) {
        return 0;
    }
    
    vec->datos = nuevosDatos;
    vec->capacidad = nuevaCapacidad;
    return 1;
}

// Agrega un elemento al final del vector
int vectorAgregar(Vector *vec, const void *elemento) {
    if (!vec || !elemento) {
        return 0;
    }
    
    // Si está lleno, duplicar capacidad
    if (vec->cantidad >= vec->capacidad) {
        if (!vectorRedimensionar(vec, vec->capacidad * 2)) {
            return 0;
        }
    }
    
    // Copiar el elemento
    void *destino = (char*)vec->datos + (vec->cantidad * vec->tamElemento);
    memcpy(destino, elemento, vec->tamElemento);
    vec->cantidad++;
    
    return 1;
}

// Obtiene un puntero al elemento en el índice especificado
void* vectorObtener(Vector *vec, size_t indice) {
    if (!vec || indice >= vec->cantidad) {
        return NULL;
    }
    
    return (char*)vec->datos + (indice * vec->tamElemento);
}

// Establece un elemento en un índice específico
int vectorEstablecerEnIndice(Vector *vec, size_t indice, const void *elemento) {
    if (!vec || !elemento || indice >= vec->cantidad) {
        return 0;
    }
    
    void *destino = (char*)vec->datos + (indice * vec->tamElemento);
    memcpy(destino, elemento, vec->tamElemento);
    
    return 1;
}

// Retorna la cantidad de elementos
size_t vectorCantidad(const Vector *vec) {
    return vec ? vec->cantidad : 0;
}

// Retorna la capacidad del vector
size_t vectorCapacidad(const Vector *vec) {
    return vec ? vec->capacidad : 0;
}

// Mezcla los elementos del vector (Fisher-Yates)
void vectorMezclar(Vector *vec) {
    if (!vec || vec->cantidad <= 1) {
        return;
    }
    
    void *temp = malloc(vec->tamElemento);
    if (!temp) {
        return;
    }
    
    for (size_t i = vec->cantidad - 1; i > 0; i--) {
        size_t j = rand() % (i + 1);
        
        void *elem_i = (char*)vec->datos + (i * vec->tamElemento);
        void *elem_j = (char*)vec->datos + (j * vec->tamElemento);
        
        // Intercambiar elementos
        memcpy(temp, elem_i, vec->tamElemento);
        memcpy(elem_i, elem_j, vec->tamElemento);
        memcpy(elem_j, temp, vec->tamElemento);
    }
    
    free(temp);
}

// Vacía el vector (mantiene la capacidad)
void vectorVaciar(Vector *vec) {
    if (vec) {
        vec->cantidad = 0;
    }
}

// Destruye el vector y libera memoria
void vectorDestruir(Vector *vec) {
    if (vec) {
        if (vec->datos) {
            free(vec->datos);
        }
        free(vec);
    }
}
