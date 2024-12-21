/**
 * @author Andrej Bartulin, Stjepan Bilić Matišić
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Header file for GUI structures
*/

#pragma once

#include "argument.h"
#include "error.h"

#include "gui_elements/button.h"
#include "gui_elements/context_menu.h"

#include "../filesystem/disk.h"

#define BLACK   0
#define BLUE    1
#define GREEN   2
#define CYAN    3
#define RED     4
#define MAGENTA 5
#define BROWN   6
#define LGRAY   7
#define DGRAY   8
#define LBLUE   9
#define LGREN   10
#define LCYAN   11
#define LRED    12
#define LMAG    13
#define YELLOW  14
#define WHITE   15

#define NUMBER_OF_BUTTONS 16

typedef struct elements_T
{
    Button button[NUMBER_OF_BUTTONS];
} Elements;

typedef struct window_T
{
    bool debug;

    Error error;
    Elements elements;

    uint16_t x;
    uint16_t y;

    uint16_t len_x;
    uint16_t len_y;

    uint16_t background_color;
    uint16_t foreground_color;

    char *title;
} Window;

typedef struct screen_T
{
    uint16_t len_x;
    uint16_t len_y;

    Window windows[7];
    int active_window;

    Argument* argument;
    DISK diskA;
} Screen;
