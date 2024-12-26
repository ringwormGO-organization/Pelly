/**
 * @author Andre Bartulin, nanobyte (https://github.com/chibicitiberiu)
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Main C file for e820 memory map
*/

#include "e820.h"

E820MemoryBlock array[100];

#define GET_SEGMENT(address) ((unsigned short)(((unsigned long)(address)) >> 16))
#define GET_OFFSET(address) ((unsigned short)((unsigned long)(address) & 0xFFFF))

E820MemoryBlock* memory_map = 0;
void detect_memory()
{
    e820(array);

    void* address = (segment * 16) + offset;
    memory_map = (E820MemoryBlock*)(address);

    printf("\r\n --- Memory map: --- \r\n\r\n");
    printf("Base | Length | Type \r\n");

    for (uint16_t i = 0;i < entry_count; i++) 
    {
        printf("0x%llx | 0x%llx | %d\r\n", memory_map[i].Base, memory_map[i].Length, memory_map[i].Type);            
    }
}
