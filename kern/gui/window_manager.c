/**
 * @author Andrej Bartulin, Stjepan Bilić Matišić
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Main C file for window manager
*/

#include "window_manager.h"

void vm_open_window(Screen* screen, WindowManager* wm, int new_window_id)
{
    wm->current_window_id = new_window_id;

    Window* window_manager = screen->windows.array[wm->window_manager_id];
    clear_window(*window_manager);

    Window* new_window = screen->windows.array[wm->current_window_id];
    draw_window(*new_window);
    draw_window_elements(*new_window, new_window_id);
}

void vm_close_window(Screen* screen, WindowManager* wm, int window_id)
{
    wm->current_window_id = wm->window_manager_id;

    Window* close_window = screen->windows.array[window_id];
    clear_window(*close_window);

    Window* window_manager = screen->windows.array[wm->window_manager_id];
    draw_window(*window_manager);
    draw_window_elements(*window_manager, wm->window_manager_id);
}

void vm_quit(WindowManager* wm)
{

}
