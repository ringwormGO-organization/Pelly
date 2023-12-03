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

/**
 * Move "any" cursor
 * @param x
 * @param y
 * @return `Cursor`
*/
Cursor cursor_move_cursor(uint16_t x, uint16_t y);
