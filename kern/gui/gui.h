#pragma once

#include "../io.h"
#include "../stdio.h"
#include "../stdint.h"

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

typedef enum error_T
{
    NO_ERROR,
    TITLE_BOUNDARY_EXCEEDED,
    X_BOUNDARY_EXCEEDED,
    Y_BOUNDARY_EXCEEDED,
} Error;

/* ------------------------------------ */

typedef struct cursor_T
{
    Error error;

    uint16_t x;
    uint16_t y;
} Cursor;

typedef struct window_T
{
    Error error;

    uint16_t x;
    uint16_t y;

    uint16_t len_x;
    uint16_t len_y;

    uint16_t background_color;
    uint16_t foreground_color;

    char *title;
} Window;

Cursor init_cursor(uint16_t x, uint16_t y);
Cursor move_cursor(uint16_t x, uint16_t y);

Window init_window(uint16_t x, uint16_t y, uint16_t len_x, uint16_t len_y, uint16_t background_color, uint16_t foreground_color, char* title, bool debug);
void draw_window(Window window);

void start_gui();
