#pragma once

#include "../../io.h"
#include "../../stdio.h"
#include "../../stdint.h"

#include "../cursor.h"
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
} Button;

Button init_button();
void draw_button(Button button);
