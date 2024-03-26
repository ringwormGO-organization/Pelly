/**
 * @author Andrej Bartulin, Stjepan Bilić Matišić
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Main header file for IO
*/

#pragma once

#include "stdint.h"
#include "stdio.h"

void _cdecl move_keyboard(uint16_t row, uint16_t column);

void _cdecl asm_keyboard_loop();

void _cdecl up_cursor();
void _cdecl down_cursor();
void _cdecl left_cursor();
void _cdecl right_cursor();

void _cdecl get_cursor_position();
void _cdecl clear_screen();

uint16_t format_color(uint16_t background_color, uint16_t foreground_color);
void _cdecl change_color(uint16_t color, uint16_t count);

void c_keyboard_loop();
