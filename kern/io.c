
#include "io.h"

#include "gui/gui.h"

int _cdecl cursor_x;
int _cdecl cursor_y;

int _cdecl ascii_code;

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
            
            case 4:
                get_cursor_position();

                if (cursor_x == x && cursor_y == y) 
                {
                    clear_window(window);
                }

                else 
                {
                    enterk();

                    /* Update global cursor */
                    move_cursor(0, global_cursor.y + 1);
                }

                break;

            default:
                printf("%c", ascii_code);

                /* Update global cursor */
                move_cursor(global_cursor.x + 1, global_cursor.y);

                break;
        }
    }
}
