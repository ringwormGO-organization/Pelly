/**
 * @author Andrej Bartulin, Stjepan Bilić Matišić
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Main C file for IO
*/

#include "io.h"

#include "gui/gui.h"

int _cdecl cursor_x;
int _cdecl cursor_y;

int _cdecl ascii_code;

/**
 * Check if enter key has been pressed on button
 * @param screen screen containing all windows containing all buttons
*/
int keyboard_event(Screen screen)
{
    for (int i = 0; i < NUMBER_OF_WINDOWS; i++)
    {
        /* Check if close window button is pressed */
        if (global_cursor.x == screen.windows[i].x + 1 && global_cursor.y == screen.windows[i].y)
        {
            clear_window(screen.windows[i]);
            continue;
        }

        for (int j = 0; j < NUMBER_OF_BUTTONS; j++)
        {
            if (global_cursor.x > screen.windows[i].x + 
                screen.windows[i].elements.button[j].x 
                && 
                global_cursor.x < screen.windows[i].x + 
                screen.windows[i].elements.button[j].x + 
                screen.windows[i].elements.button[j].len_x)
            {
                if (global_cursor.y > screen.windows[i].y + 
                screen.windows[i].elements.button[j].y 
                && 
                global_cursor.y < screen.windows[i].y + 
                screen.windows[i].elements.button[j].y + 
                screen.windows[i].elements.button[j].len_y)
                {
                    /* execute a function */
                    screen.windows[i].elements.button[j].action();
                }
            }
        }
    }


    return -1;
}

/**
 * Main keyboard loop
 * @param screen screen for checking events
*/
void c_keyboard_loop(Screen screen) 
{
    while (1)
    {
        /* Get pressed key from assembly */
        asm_keyboard_loop();
        
        /* Check ASCII code */
        switch (ascii_code)
        {
            case 0:
                /* Move real, assembly cursor */
                up_cursor();

                /* Update global cursor */
                get_cursor_position();
                move_cursor(cursor_x, cursor_y);

                break;

            case 1:
                /* Move real, assembly cursor */
                down_cursor();

                /* Update global cursor */
                get_cursor_position();
                move_cursor(cursor_x, cursor_y);

                break;

            case 2:
                /* Move real, assembly cursor */
                left_cursor();

                /* Update global cursor */
                get_cursor_position();
                move_cursor(cursor_x, cursor_y);


                break;

            case 3:
                /* Move real, assembly cursor */
                right_cursor();

                /* Update global cursor */
                get_cursor_position();
                move_cursor(cursor_x, cursor_y);

                break;

            default:
                /* Print character (disabled) */
                // printf("%c", ascii_code);

                /* Enter key*/
                if (ascii_code == 13)
                {
                    keyboard_event(screen);

                    /* Update global cursor (disabled because we are not printing a character) */
                    // move_cursor(0, global_cursor.y + 1);
                }

                else if (ascii_code == 8)
                {
                    /* Update global cursor */
                    move_cursor(global_cursor.x - 1, global_cursor.y);
                }

                else
                {
                    /* Update global cursor (disabled because we are not printing a character) */
                    // move_cursor(global_cursor.x + 1, global_cursor.y);
                }

                break;
        }
    }
}
