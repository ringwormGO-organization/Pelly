#include "e820.h"

#define MAX_REGIONS 256

MemoryRegion g_MemRegions[MAX_REGIONS];
int g_MemRegionCount;

void get_memory_map()
{
    E820MemoryBlock block;
    uint32_t continuation = 0;
    int ret;
    
    g_MemRegionCount = 0;
    ret = E820GetNextBlock(&block, &continuation);

    printf("%d | %d\r\n", ret, continuation);

    while (ret > 0 && continuation != 0)
    {
        g_MemRegions[g_MemRegionCount].Begin = block.Base;
        g_MemRegions[g_MemRegionCount].Length = block.Length;
        g_MemRegions[g_MemRegionCount].Type = block.Type;
        g_MemRegions[g_MemRegionCount].ACPI = block.ACPI;
        ++g_MemRegionCount;

        // printf("E820: base=0x%llx length=0x%llx type=0x%x\r\n", block.Base, block.Length, block.Type);
        printf("%x", block.Base);

        ret = E820GetNextBlock(&block, &continuation);
    }

    // fill meminfo structure
    // memoryInfo->RegionCount = g_MemRegionCount;
    // memoryInfo->Regions = g_MemRegions;
}

MemoryMapEntry* memMapArr = 0; 
void detect_memory()
{
    E820MemoryBlock array[100];
    e820(&array);

    for (uint16_t i = 0; i < entry_count; i++) 
    {
        printf("%d\r\n", array->Length);
    }
}
