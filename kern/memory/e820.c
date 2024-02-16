#include "e820.h"

E820MemoryBlock array[100];
// E820MemoryBlock* array = (E820MemoryBlock*)0x2FFF;
void detect_memory()
{
    e820(array);
    
    for (uint16_t i = 0; i < entry_count; i++) 
    {
        printf("%d: %llx | %llx\r\n", i, array[i].Base, array[i].Length);
    }
}
