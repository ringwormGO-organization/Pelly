#include "vector.h"

void vector_new(vector_t *a, size_t initialSize) 
{
    a->array = malloc(initialSize * sizeof(int));
    a->used = 0;
    a->size = initialSize;
}

void vector_pushback(vector_t *a, void* element) 
{
    // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
    // Therefore a->used can go up to a->size 
    if (a->used == a->size) 
    {
        a->size *= 2;
        a->array = realloc(a->array, a->size * sizeof(int));
    }

    a->array[a->used++] = element;
}

void vector_free(vector_t *a) 
{
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}
