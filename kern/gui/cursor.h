#pragma once

#include "../io.h"
#include "../stdio.h"
#include "../stdint.h"

#include "error.h"

typedef struct cursor_T
{
    Error error;

    uint16_t x;
    uint16_t y;
} Cursor;
