/**
 * @author Andrej Bartulin, Stjepan Bilić Matišić
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Main header file for GUI
*/

#pragma once

#include "../io/io.h"
#include "../memory/memory.h"
#include "../stdio.h"
#include "../stdint.h"

#include "gui_structures.h"

/**
 * Create a button
 * @param x starting position (x)
 * @param y starting position (y)
 * @param len_x width of a button
 * @param len_y height of a button
 * @param background_color background color of a button
 * @param foreground_color foreground color of a button
 * @param title title of a button
 * @param action function that will get executed when button is pressed
*/
Button create_button(uint16_t x, uint16_t y, uint16_t len_x, uint16_t len_y, 
                    uint16_t background_color, uint16_t foreground_color, char* title, void (*action)());

/**
 * Check can a button be drawn
 * @param window window where button will be checked
 * @param button button to be checked
*/
void check_button(Window* window, Button* button);

/**
 * Draw a button
 * @param window window where button will be drawn
 * @param button button which will be drawn
*/
void draw_button(Window* window, Button button);

/* ------------------------------------------------------------------------- */

/**
 * Initialize a window
 * @param x starting position (x)
 * @param y starting position (y)
 * @param len_x width of the a window
 * @param len_y height of the a window
 * @param background_color background color of a window
 * @param foreground_color foreground color of a window
 * @param title title of a window
 * @param debug debug mode?
*/
Window create_window(uint16_t x, uint16_t y, uint16_t len_x, uint16_t len_y, 
                    uint16_t background_color, uint16_t foreground_color, char* title, bool debug);

/**
 * Initializes buttons in a window by assigning them error value `EMPTY`
 * @param window window where buttons will be initialized
 */
void initialize_buttons(Window* window);

/**
 * Check can a window be drawn
 * @param screen screen where window will be checked
 * @param window window to be checked
*/
void check_window(Screen screen, Window* window);

/**
 * Draw a window
 * @param window window to be drawn
*/
void draw_window(Window* window);

/**
 * Draw window elements (buttons, context menus, ...)
 * @param window window where elements will be drawn
 * @param window_id window's id in `Screen` type
*/
void draw_window_elements(Window* window, int window_id);

/**
 * Clear a given window
 * @param window window to be cleaned
*/
void clear_window(Window* window);

/* ------------------------------------------------------------------------- */

/**
 * Entry point for GUI
 * @param diskA diskA (FAT disk)
*/
void start_gui(DISK diskA);
