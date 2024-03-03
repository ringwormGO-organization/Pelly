/**
 * @author Kevin Alavik (https://github.com/KevinAlavik)
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Main header file for cursor
*/

#include "allocator.h"

uint64_t div_round_up(uint64_t x, uint64_t y)
{
    uint64_t number = x + (y - 1);
    int radix = y;
    uint32_t rem;

    x86_div64_32(number, radix, &number, &rem);
    return number;
}

uint64_t align_up(uint64_t x, uint64_t y)
{
    uint64_t result;
    x86_mul64_64(div_round_up(x, y), y, &result);

    return result;
}

uint64_t align_down(uint64_t x, uint64_t y)
{
    uint64_t number = x;
    int radix = y;
    uint32_t rem;

    x86_div64_32(number, radix, &number, &rem);

    uint64_t result;
    x86_mul64_64(number, y, &result);

    return result;
}

void bitmap_set(uint8_t* bitmap, uint64_t bit) {
    uint32_t rem;
    int radix = 8;

    x86_div64_32(bit, radix, &bit, &rem);
    bitmap[bit] |= 1 << (rem);
}

void bitmap_clear(uint8_t* bitmap, uint64_t bit) {
    uint32_t rem;
    int radix = 8;

    x86_div64_32(bit, radix, &bit, &rem);
    bitmap[bit] &= ~(1 << (rem));
}

uint8_t bitmap_get(uint8_t* bitmap, uint64_t bit) {
    uint32_t rem;
    int radix = 8;

    x86_div64_32(bit, radix, &bit, &rem);
    return bitmap[bit] & (1 << (rem));
}

E820MemoryBlock* memory_map = 0;
void init_pmm()
{
    void* address = (segment * 16) + offset;
    memory_map = (E820MemoryBlock*)(address);

    uint64_t top_address;
    uint64_t higher_address = 0;

    for (uint16_t entry = 0; entry < entry_count; entry++) 
    {
        if (memory_map[entry].Type == 1)
        {
            top_address = memory_map[entry].Base + memory_map[entry].Length;
            if (top_address > higher_address)
            {
                higher_address = top_address;
            }
        }
    }

    uint64_t number = 0;
    int radix = 1;
    uint32_t rem;

    /* ------------ */

    number = higher_address;
    radix = PAGE_SIZE;

    x86_div64_32(number, radix, &number, &rem);
    bitmap_pages = number;

    /* ------------ */

    number = bitmap_pages;
    radix = 8;

    x86_div64_32(number, radix, &number, &rem);
    bitmap_size = align_up(number, PAGE_SIZE);

    update_memory();

    for (uint16_t entry = 0; entry < entry_count; entry++) 
    {
        if (memory_map[entry].Type == 1) 
        {
            if (memory_map[entry].Length >= bitmap_size) 
            {
                bitmap = memory_map[entry].Base;
                memset(bitmap, 0xFF, bitmap_size);

                memory_map[entry].Base += bitmap_size;
                memory_map[entry].Length -= bitmap_size;

                break;
            }
        }
    }

    for (uint16_t entry = 0; entry < entry_count; entry++) 
    {
        if (memory_map[entry].Type == 1) 
        {
            for (uint64_t i = 0; i < memory_map[entry].Length; i += PAGE_SIZE) 
            {
                number = memory_map[entry].Base + i;
                radix = PAGE_SIZE;

                x86_div64_32(number, radix, &number, &rem);
                bitmap_clear(bitmap, number);
            }
        }
    }
}

void update_memory() {
    free_memory = 0;

    for (uint16_t entry = 0; entry < entry_count; entry++) 
    {
        if (memory_map[entry].Type == 1) 
        {
            free_memory += memory_map[entry].Length;
        }
    }
}

void* pmm_request_page() {
  uint64_t last_bit_val;
  uint64_t last_allocated_index;

    while (1) 
    {
        last_bit_val = bitmap_get(bitmap, last_allocated_index);

        if (last_bit_val == 0)
        {
            uint64_t result;
            x86_mul64_64(last_allocated_index, PAGE_SIZE, &result);

            return (void *)(result);
        }

        else
        {
            if (last_allocated_index > bitmap_pages)
            {
                return NULL;
            }
            
            else
            {
                last_allocated_index++;
            }
        }
    }
}

void* pmm_request_pages(size_t numPages) 
{
    uint64_t last_allocated_index = 0;

    // Loop until we find consecutive free pages
    while (1) 
    {
        // Check if the current page is free
        if (!bitmap_get(bitmap, last_allocated_index)) 
        {
            size_t consecutive_free_pages = 1;

            // Check if consecutive pages are free
            for (size_t i = 1; i < numPages; ++i) 
            {
                if (!bitmap_get(bitmap, last_allocated_index + i)) 
                {
                    ++consecutive_free_pages;
                }

                else 
                {
                    // If any page in the sequence is not free, reset the counter
                    consecutive_free_pages = 0;
                    break;
                }
            }

            // If we found enough consecutive free pages, allocate them
            if (consecutive_free_pages == numPages) 
            {
                // Mark the pages as allocated in the bitmap
                for (size_t i = 0; i < numPages; ++i) 
                {
                    bitmap_set(bitmap, last_allocated_index + i);
                }

                // Return a pointer to the first page

                uint64_t result;
                x86_mul64_64(last_allocated_index, PAGE_SIZE, &result);

                return (void *)(result);
            }
        }

        // Move to the next page
        ++last_allocated_index;

        // Check if we have reached the end of the bitmap
        if (last_allocated_index >= bitmap_pages) 
        {
            return NULL; // No consecutive free pages found
        }
    }
}

void pmm_free(void *ptr) 
{
    uint64_t number = (uint64_t)ptr;
    int radix = PAGE_SIZE;
    uint32_t rem;

    x86_div64_32(number, radix, &number, &rem);
    uint64_t bit_idx = number;

    bitmap_clear(bitmap, bit_idx);
}

void pmm_free_pages(void *ptr, size_t numPages) {
    uint64_t number = (uint64_t)ptr;
    int radix = PAGE_SIZE;
    uint32_t rem;

    x86_div64_32(number, radix, &number, &rem);
    uint64_t start_bit_idx = number;

    // Mark the pages as free in the bitmap
    for (size_t i = 0; i < numPages; ++i) 
    {
        bitmap_clear(bitmap, start_bit_idx + i);
    }
}
