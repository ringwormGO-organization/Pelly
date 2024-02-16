#include "e820.h"

void detect_memory()
{
    E820MemoryBlock array[100];
    e820(array);

    for (uint16_t i = 0; i < entry_count; i++) 
    {
        printf("%lld\r\n", array[i].Length);
    }
}
