/**
 * @author Andre Bartulin, nanobyte (https://github.com/chibicitiberiu)
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Main header file for e820 memory map
*/

/* This file is subject to Paradox's license */

#pragma once

#include "stdint.h"
#include "../stdio.h"

uint16_t _cdecl entry_count;

uint16_t _cdecl offset;
uint16_t _cdecl segment;

void _cdecl e820(void far* data);

typedef struct 
{
    uint64_t Base;
    uint64_t Length;
    uint32_t Type;
    uint32_t ACPI;
} E820MemoryBlock;

void detect_memory();
