/**
 * @author Andrej Bartulin, Stjepan Bilić Matišić
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Main header file for window manager
*/

#pragma once

#include "../stdio.h"
#include "../stdint.h"

#include "gui.h"

typedef struct window_manager_T
{
    int window_manager_id;
    int current_window_id;
} WindowManager;

/**
 * Open a window
 * @param screen Screen struct
 * @param wm window manager struct
 * @param new_window_id window to be opened
*/
void wm_open_window(Screen* screen, WindowManager* wm, int new_window_id);

/**
 * Close a window
 * @param wm window manager struct
 * @param window_id window to be closed
*/
void wm_close_window(Screen* screen, WindowManager* wm, int window_id);

/**
 * Close window manager
 * @param wm window manager struct
*/
void wm_quit(WindowManager* wm);
