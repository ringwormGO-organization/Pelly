
#include "io.h"

int _cdecl ascii_code;

void c_keyboard_loop(int x, int y, Window window) {
    while (1)
    {
        asm_keyboard_loop();
        
        switch (ascii_code)
        {
            case 0:
                up_cursor();
                break;

            case 1:
                down_cursor();
                break;

            case 2:
                left_cursor();
                break;

            case 3:
                right_cursor();
                break;
            
            case 4:
                get_cursor_position();
                if (cursor_x == x && cursor_y == y) {
                    clear_window(window);
                }
                //printf("%d | %d ", cursor_x, cursor_y);
                break;

            default:
                //printf("%c", ascii_code);
                break;
        }
    }
}
