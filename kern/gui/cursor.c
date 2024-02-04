/**
 * @author Andrej Bartulin, Stjepan Bilić Matišić
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Main C file for cursor
*/

#include "cursor.h"

Cursor cursor_move_cursor(uint16_t x, uint16_t y)
{
    Cursor new;

    /* Perform checks */
    if (x > 79)
    {
        new.error = X_BOUNDARY_EXCEEDED;
        return new;
    }

    if (y > 24)
    {
        new.error = Y_BOUNDARY_EXCEEDED;
        return new;
    }

    new.error = NO_ERROR;

    new.x = x;
    new.y = y;

    move_keyboard(new.x, new.y);
    return new;
}
