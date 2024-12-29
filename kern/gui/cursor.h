/**
 * @author Andrej Bartulin, Stjepan Bilić Matišić
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Main header file for cursor
*/

#pragma once

#include "../stdint.h"

typedef struct cursor_T
{
    uint16_t x;
    uint16_t y;
} Cursor;

/**
 * Move "any" cursor
 * @param x
 * @param y
 * @return `Cursor`
*/
Cursor move_virtual_cursor(uint16_t x, uint16_t y);
