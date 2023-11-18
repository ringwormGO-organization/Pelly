
#pragma once

#include "stdint.h"
#include "x86.h"
#include "io.h"
#include "gui/cursor.h"

struct cursor_T;

struct cursor_T global_cursor;

void putc(char c);
void puts(const char* str);
void puts_f(const char far* str);

void _cdecl printf(const char* fmt, ...);
void print_buffer(const char* msg, const void far* buffer, uint16_t count);

size_t strlen(const char* str);
