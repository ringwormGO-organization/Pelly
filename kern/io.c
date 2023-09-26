
#include "io.h"

int _cdecl ascii_code;

void c_keyboard_loop() {
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
            
            default:
                printf("%c", ascii_code);
                break;
        }
    }
}
