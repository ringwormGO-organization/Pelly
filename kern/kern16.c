
#include "stdint.h"
#include "stdio.h"
#include "io.h"
#include "filesystem/disk.h"
#include "filesystem/fat.h"

#define KEYB    0

void far* g_data = (void far*)0x00500200;

void _cdecl disk_test_write();

void _cdecl kstart_(uint16_t bootDrive)
{
    clear_screen();

    printf("===> Pelly Operating System - 0.2.1\r\n");
    printf("Now with keyboard cursor! %d\r\n", 30);

    starting_cursor_row += 2;

    DISK disk;
    if (!DISK_Initialize(&disk, bootDrive))
    {
        printf("Disk init error\r\n");
        goto end;
    }

    x86_Disk_Write(0, 64, 0, 1, 0, 0x7c00, 0x0000);
//    disk_test_write();

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
        starting_cursor_row += 1;
    }
    FAT_Close(fd);

    // read test.txt
    char buffer[100];
    uint32_t read;
    fd = FAT_Open(&disk, "doc/test.txt");
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

    void far * data_to_write = "test";   //  address where?

    starting_cursor_row += 1;

    init_keyboard(starting_cursor_row, 0);
    c_keyboard_loop();

end:
    for (;;);
}
