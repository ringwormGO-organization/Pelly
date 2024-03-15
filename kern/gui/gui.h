/**
 * @author Andrej Bartulin, Stjepan Bilić Matišić
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Main header file for GUI
*/

#pragma once

#include "../io.h"
#include "../memory/memory.h"
#include "../stdio.h"
#include "../stdint.h"

#include "../libc/vector.h"

#include "cursor.h"
#include "error.h"
#include "window_manager.h"

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

#define NUMBER_OF_PROGRAMS 6

/* ------------------------------------------------------------------------- */

typedef struct elements_T
{
    vector_t button;
    struct context_menu_T context_menu;
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

    vector_t windows;
} Screen;

/* ------------------------------------------------------------------------- */

/**
 * Initialize a button
 * @param x starting position (x)
 * @param y starting position (y)
 * @param len_x width of a button
 * @param len_y height of a button
 * @param background_color background color of a button
 * @param foreground_color foreground color of a button
 * @param title title of a button
 * @param action function that will get executed when button is pressed
*/
Button* init_button(uint16_t x, uint16_t y, uint16_t len_x, uint16_t len_y, 
                    uint16_t background_color, uint16_t foreground_color, char* title, void (*action)());

/**
 * Check if button can be drawn
 * @param window window where checks will be performed
 * @param id button's position in window's elements list 
*/
void check_button(Window* window, int id);

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
 * @param context_buttons context menu elements in form of buttons
*/
ContextMenu init_context_menu(uint16_t x, uint16_t y, uint16_t len_x, uint16_t len_y, 
                    uint16_t background_color, uint16_t foreground_color, vector_t context_buttons);

/**
 * Check if context menu can be drawn
 * @param window window where checks will be performed
 * @param context_menu context menu to be checked
*/
void check_context_menu(Window window, ContextMenu* context_menu);

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
Window* init_window(uint16_t x, uint16_t y, uint16_t len_x, uint16_t len_y, 
                    uint16_t background_color, uint16_t foreground_color, char* title, bool debug);

/**
 * Check if window can be drawn
 * @param window window to be checked
 * @param id window's position in `Screen` type
*/
void check_window(Screen* screen, int id);

/**
 * Draw a window
 * @param window window to be drawn
*/
void draw_window(Window window);

/**
 * Draw window elements (buttons, context menus, ...)
 * @param window window where elements will be drawn
 * @param window_id window's id in `Screen` type
*/
void draw_window_elements(Window window, int window_id);

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
