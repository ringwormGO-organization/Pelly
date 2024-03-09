#pragma once

#include "../stdint.h"
#include "../memory/heap.h"

typedef struct vector_t 
{
    void **array;
    size_t used;
    size_t size;
} vector_t;

void vector_new(vector_t *a, size_t initialSize);
void vector_pushback(vector_t *a, void* element);
void vector_free(vector_t *a);
