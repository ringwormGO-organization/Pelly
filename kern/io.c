
#include "io.h"

#include "gui/gui.h"

int _cdecl cursor_x;
int _cdecl cursor_y;

int _cdecl ascii_code;

/**
 * Check if enter key has been pressed on button
 * # Return table
 * -1 - enter is not pressed on button
 * 0 and rest - button on which button was pressed
*/
int keyboard_event(Window window)
{
    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        if (global_cursor.x > window.x + window.elements.button[i].x && global_cursor.x <
            window.x + window.elements.button[i].x + window.elements.button[i].len_x)
        {
            if (global_cursor.y > window.y + window.elements.button[i].y && global_cursor.y <
            window.y + window.elements.button[i].y + window.elements.button[i].len_y)
            {
                return i;
            }
        }
    }

    return -1;
}

void c_keyboard_loop(int x, int y, Window window) 
{
    while (1)
    {
        asm_keyboard_loop();
        
        switch (ascii_code)
        {
            case 0:
                up_cursor();

                /* Update global cursor */
                get_cursor_position();
                move_cursor(cursor_x, cursor_y);

                break;

            case 1:
                down_cursor();

                /* Update global cursor */
                get_cursor_position();
                move_cursor(cursor_x, cursor_y);

                break;

            case 2:
                left_cursor();

                /* Update global cursor */
                get_cursor_position();
                move_cursor(cursor_x, cursor_y);


                break;

            case 3:
                right_cursor();

                /* Update global cursor */
                get_cursor_position();
                move_cursor(cursor_x, cursor_y);

                break;

            default:
                // printf("%c", ascii_code);

                /* Update global cursor */
                if (ascii_code == 13)
                {
                    get_cursor_position();

                    if (global_cursor.x == x && global_cursor.y == y)
                    {
                        clear_window(window);
                    }

                    else if (keyboard_event(window) > -1)
                    {
                        /* execute function */
                    }

                    else
                    {
                        // move_cursor(0, global_cursor.y + 1);
                    }
                }

                else if (ascii_code == 8)
                {
                    move_cursor(global_cursor.x - 1, global_cursor.y);
                }

                else
                {
                    // move_cursor(global_cursor.x + 1, global_cursor.y);
                }

                break;
        }
    }
}
