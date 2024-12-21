/**
 * @author Andrej Bartulin, Stjepan Bilić Matišić
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Main C file for IO
*/

#include "io.h"

#include "libc/string.h"

int _cdecl cursor_x;
int _cdecl cursor_y;

int _cdecl ascii_code;

/**
 * Format color so it can be used properly in `change_color` function
 * @param background_color current background color
 * @param foreground_color foreground_color
*/
uint16_t format_color(uint16_t background_color, uint16_t foreground_color)
{
    return (background_color * 16) + foreground_color;
}

/**
 * Main keyboard loop
 * @param screen screen for checking events
*/
void c_keyboard_loop(Screen* screen) 
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

                break;

            case 1:
                /* Move real, assembly cursor */
                down_cursor();

                break;

            case 2:
                /* Move real, assembly cursor */
                left_cursor();

                break;

            case 3:
                /* Move real, assembly cursor */
                right_cursor();

                break;

            default:
                /* Enter key */
                if (ascii_code == 13)
                {
                    /* Print character (disabled) */
                    // printf("\r\n");

                    /* Update global cursor (disabled because we are not printing a character) */
                    // get_real_cursor_position();
                    // move_real_cursor(cursor_x, cursor_y);

                    enter_event(screen);
                }

                /* Backspace key */
                else if (ascii_code == 8)
                {
                    /* Print character (disabled) */
                    // printf("\b ");

                    /* Update global cursor */
                    // get_real_cursor_position();
                    // move_real_cursor(cursor_x - 1, cursor_y);

                    backspace_event(screen);
                }

                else
                {
                    /* Print character (disabled) */
                    // printf("%c", ascii_code);

                    /* Update global cursor (disabled because we are not printing a character) */
                    // get_real_cursor_position();
                    // move_real_cursor(cursor_x, cursor_y);

                    character_event(screen, ascii_code);
                }

                break;
        }
    }
}
