#pragma once

#include "../io.h"
#include "../stdio.h"
#include "../stdint.h"

#include "cursor.h"
#include "error.h"

#include "gui_elements/button.h"
#include "gui_elements/context_menu.h"

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

#define BUTTON_SIZE 4

/* ------------------------------------------------------------------------- */

typedef struct elements_T
{
    struct button_T button[BUTTON_SIZE];
    struct context_menu_T context_menu;
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

/* ------------------------------------------------------------------------- */

/**
 * Initialize a button
 * @param window window element where button will be drawn
 * @param x starting position (x)
 * @param y starting position (y)
 * @param len_x width of a button
 * @param len_y height of a button
 * @param background_color background color of a button
 * @param foreground_color foreground color of a button
 * @param title title of a button
*/
Button init_button(Window window, uint16_t x, uint16_t y, uint16_t len_x, uint16_t len_y, 
                    uint16_t background_color, uint16_t foreground_color, char* title);

/**
 * Draw a button
 * @param window window where button will be drawn
 * @param button button which will be drawn
*/
void draw_button(Window window, Button button);

/* ------------------------------------*/

/**
 * Initialize a context menu
 * @param window window element where button will be drawn
 * @param x starting position (x)
 * @param y starting position (y)
 * @param len_x width
 * @param len_y height
 * @param background_color background color
 * @param foreground_color foreground color
 * @param buttons context menu elements in form of buttons
*/
ContextMenu init_context_menu(Window window, uint16_t x, uint16_t y, uint16_t len_x, uint16_t len_y, 
                    uint16_t background_color, uint16_t foreground_color, Button buttons[BUTTON_SIZE]);

/**
 * Draw a context menu
 * @param window window where button will be drawn
 * @param button button which will be drawn
*/
void draw_context_menu(Window window, ContextMenu context_menu);

/* ------------------------------------------------------------------------- */

/**
 * Initialize a window
 * @param window window element where button will be drawn
 * @param x starting position (x)
 * @param y starting position (y)
 * @param len_x width of the a window
 * @param len_y height of the a window
 * @param background_color background color of a window
 * @param foreground_color foreground color of a window
 * @param title title of a window
 * @param debug debug mode?
*/
Window init_window(uint16_t x, uint16_t y, uint16_t len_x, uint16_t len_y, 
                    uint16_t background_color, uint16_t foreground_color, char* title, bool debug);

/**
 * Draw a window
 * @param window window to be drawn
*/
void draw_window(Window window);

/**
 * Draw window elements (buttons, context menus, ...)
 * @param window window where elements will be drawn
 * @param elements elements to draw
 * @param debug debug mode?
*/
void draw_window_elements(Window window, bool debug);

/**
 * Clear a given window
 * @param window window to be cleaned
*/
void clear_window(Window window);

/* ------------------------------------------------------------------------- */

/**
 * Entry point for GUI
*/
void start_gui();
