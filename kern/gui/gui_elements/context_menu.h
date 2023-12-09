#pragma once

#include "../../stdio.h"
#include "../../stdint.h"

#include "../error.h"
#include "button.h"

#define BUTTON_SIZE 4

typedef struct context_menu_T
{
    Error error;

    uint16_t x;
    uint16_t y;

    uint16_t len_x;
    uint16_t len_y;

    uint16_t background_color;
    uint16_t foreground_color;

    struct button_T buttons[BUTTON_SIZE];
} ContextMenu;
