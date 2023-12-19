#pragma once

#include "../../stdio.h"
#include "../../stdint.h"

#include "../error.h"
#include "button.h"

#define NUMBER_OF_BUTTONS 4

typedef struct context_button_T
{
    Error error;

    uint16_t len_y;
    char* title;
} ContextButton;

typedef struct context_menu_T
{
    Error error;

    uint16_t x;
    uint16_t y;

    uint16_t len_x;
    uint16_t len_y;

    uint16_t background_color;
    uint16_t foreground_color;

    struct context_button_T buttons[NUMBER_OF_BUTTONS];
} ContextMenu;
