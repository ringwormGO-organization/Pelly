#pragma once

#include "stdint.h"
#include "../stdio.h"

int _cdecl low_memory;
int _cdecl used_memory;
uint16_t _cdecl entry_count;

void _cdecl get_low_memory();
void _cdecl get_used_memory();

void _cdecl e820(void far* data);

typedef struct 
{
    uint64_t Base;
    uint64_t Length;
    uint32_t Type;
    uint32_t ACPI;
} E820MemoryBlock;

int _cdecl E820GetNextBlock(E820MemoryBlock* block, uint32_t* continuationId);

void get_memory_map();
void detect_memory();
