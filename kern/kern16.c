
#include "stdint.h"
#include "stdio.h"
#include "disk.h"
#include "fat.h"

#define KEYB    0

void far* g_data = (void far*)0x00500200;

void _cdecl keyboard_asm();
void _cdecl keyb_mouse();
void _cdecl clear_screen();

int _cdecl ascii_code;

void _cdecl kstart_(uint16_t bootDrive)
{
    clear_screen();

    printf("===> Pelly Operating System - 0.0.9\r\n");
    printf("Now with keyboard controled mouse! %d\r\n", 30);

    printf("--- TEST --- Variable 'ascii_code' from assembly is: %d\r\n\r\n", ascii_code);

    DISK disk;
    if (!DISK_Initialize(&disk, bootDrive))
    {
        printf("Disk init error\r\n");
        goto end;
    }

    DISK_ReadSectors(&disk, 19, 1, g_data);

    if (!FAT_Initialize(&disk))
    {
        printf("FAT init error\r\n");
        goto end;
    }

    // browse files in root
    FAT_File far* fd = FAT_Open(&disk, "/");
    FAT_DirectoryEntry entry;
    int i = 0;
    while (FAT_ReadEntry(&disk, fd, &entry) && i++ < 5)
    {
        printf("  ");
        for (int i = 0; i < 11; i++)
            putc(entry.Name[i]);
        printf("\r\n");
    }
    FAT_Close(fd);

    // read test.txt
    char buffer[100];
    uint32_t read;
    fd = FAT_Open(&disk, "test.txt");
    while ((read = FAT_Read(&disk, fd, sizeof(buffer), buffer)))
    {
        for (uint32_t i = 0; i < read; i++)
        {
            if (buffer[i] == '\n')
                putc('\r');
            putc(buffer[i]);
        }
    }
    FAT_Close(fd);

    keyboard_asm();
    /* keyb_mouse(); */

end:
    for (;;);
}
