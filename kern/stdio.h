/**
 * @author Andrej Bartulin, Stjepan Bilić Matišić, nanobyte
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Main header file for standard input and output
*/

#pragma once

#include "stdint.h"
#include "x86.h"
#include "io.h"

#include "gui/cursor.h"
#include "gui/error.h"

struct cursor_T;

/**
 * Virtual cursor
*/
struct cursor_T global_cursor;

/**
 * Move virtual and real cursor
 * @param x
 * @param y
*/
void move_cursor(uint16_t x, uint16_t y);

void putc(char c);
void puts(const char* str);
void puts_f(const char far* str);

void _cdecl printf(const char* fmt, ...);
void print_buffer(const char* msg, const void far* buffer, uint16_t count);

size_t strlen(const char* str);

void separate_bases(uint64_t number, uint16_t parts[4]);
void x86_mul64_64(uint64_t first_factor, uint64_t second_factor, uint64_t* result);
