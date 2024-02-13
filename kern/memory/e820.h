#pragma once

int _cdecl low_memory;
int _cdecl used_memory;

void _cdecl get_low_memory();
void _cdecl get_used_memory();

void _cdecl do_e820();
