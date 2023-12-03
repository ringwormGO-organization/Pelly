#pragma once

#include "../io.h"
#include "../stdio.h"
#include "../stdint.h"

#include "cursor.h"
#include "error.h"

#include "gui_elements/button.h"

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

/* ------------------------------------ */

typedef struct elements_T
{
    struct button_T button[16];
} Elements;

typedef struct window_T
{
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

Window init_window(uint16_t x, uint16_t y, uint16_t len_x, uint16_t len_y, 
                    uint16_t background_color, uint16_t foreground_color, char* title, 
                    Elements elements, bool debug);
void draw_window(Window window);
void clear_window(Window window);

void start_gui();
