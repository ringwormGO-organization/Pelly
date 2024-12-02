/**
 * @author Andrej Bartulin, Stjepan Bilić Matišić
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Main C file for cursor
*/

#include "cursor.h"

Cursor move_virtual_cursor(uint16_t x, uint16_t y)
{
    Cursor new;

    /* Perform checks */
    if (x > 79)
    {
        new.x = 0;

        if (y > 24)
        {
            new.y = 0;
        }

        else
        {
            new.y++;
        }

        return new;
    }

    if (y > 24)
    {
        new.y = 0;
        return new;
    }

    new.x = x;
    new.y = y;

    return new;
}
