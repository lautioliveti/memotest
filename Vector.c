#include "vector.h"

bool crearVector(Vector* v, size_t tamElem, size_t cap)
{
    v->cap = cap;
    v->vec = malloc(v->cap * tamElem);
    if(!v->vec)
        return false;
    v->tamElem = tamElem;
    v->ce = 0;

    return true;
}

void destruirVector(Vector* v)
{
    if (!v)
        return;
    free(v->vec);
}

int tamVector(const Vector* v)
{
    if (!v)
        return 0;
    return (int)v->ce;
}

void* obtenerVector(Vector* v, int index)
{
    if (!v || !v->vec)
        return NULL;
    if (index < 0 || index >= (int)v->ce)
        return NULL;

    return (char*)v->vec + (index * v->tamElem);
}


int redimensionarVector(Vector* v, size_t capMinima)
{
    if (!v)
        return -1;

    if (v->cap >= capMinima)
        return 0;

    size_t capNueva = v->cap == 0 ? CAP_INICIAL : v->cap;

    while (capNueva < capMinima)
    {
        capNueva *= 2;
    }

    void* vecNuevo = realloc(v->vec, capNueva * v->tamElem);
    if (!vecNuevo) return -1;

    v->vec = vecNuevo;
    v->cap = capNueva;

    return 0;
}

int insertarFinalVector(Vector* v, const void* elem)
{
    if (v->ce == v->cap)
    {
        if (redimensionarVector(v, v->cap + 1) != 0)
        {
            return -1;
        }
    }

    void* dest = (char*)v->vec + (v->ce * v->tamElem);
    memcpy(dest, elem, v->tamElem);

    v->ce++;
    return 0;
}
