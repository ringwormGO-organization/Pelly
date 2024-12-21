/**
 * @author Andrej Bartulin, Stjepan Bilić Matišić
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Header file for button GUI element
*/

#pragma once

#include "../../stdio.h"
#include "../../stdint.h"

#include "../error.h"

typedef struct button_T
{
    Error error;

    uint16_t x;
    uint16_t y;

    uint16_t len_x;
    uint16_t len_y;

    uint16_t background_color;
    uint16_t foreground_color;

    char* title;
    void (*action)();
} Button;
