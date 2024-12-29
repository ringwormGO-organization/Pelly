/**
 * @author Kevin Alavik (https://github.com/KevinAlavik)
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Main header file for heap
*/

/* This file is subject to Paradox's license */

#pragma once

#include "../stdio.h"
#include "../stdint.h"

#include "allocator.h"

void init_malloc();

void *malloc(size_t size);
void *calloc(size_t size);
void *realloc(void *old, size_t size);
void free(void *to_free);
