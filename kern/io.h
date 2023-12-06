
#pragma once

#include "stdint.h"
#include "stdio.h"

void _cdecl move_keyboard(uint16_t row, uint16_t column);

void _cdecl asm_keyboard_loop();
void _cdecl enterk();

void _cdecl up_cursor();
void _cdecl down_cursor();
void _cdecl left_cursor();
void _cdecl right_cursor();

void _cdecl get_cursor_position();
void _cdecl clear_screen();

void c_keyboard_loop();
