
#pragma once
#include "stdint.h"
#include "stdio.h"

int _cdecl ascii_code;

void _cdecl init_keyboard();
void _cdecl asm_keyboard();
void _cdecl clear_screen();

void _cdecl up_cursor();
void _cdecl down_cursor();
void _cdecl left_cursor();
void _cdecl right_cursor();

void keyboard_loop() {
    while (1)
    {
        asm_keyboard();
        
        switch (ascii_code)
        {
            case '0':
                up_cursor();
                break;

            case '1':
                down_cursor();
                break;

            case '2':
                left_cursor();
                break;

            case '3':
                right_cursor();
                break;
            
            default:
                printf("%c", ascii_code);
                break;
        }
    }
}
