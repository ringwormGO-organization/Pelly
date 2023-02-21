
#include "stdint.h"
#include "stdio.h"
#include "filesystem/disk.h"
#include "filesystem/fat.h"

#define KEYB    0

void far* g_data = (void far*)0x00500200;

void _cdecl c_keyboard();
void _cdecl clear_screen();

int _cdecl ascii_code;

void _cdecl move_cursor_up();
void _cdecl move_cursor_down();
void _cdecl move_cursor_left();
void _cdecl move_cursor_right();

void _cdecl kstart_(uint16_t bootDrive)
{
    clear_screen();

    printf("===> Pelly Operating System - 0.1.5\r\n");
    printf("Now with keyboard cursor! %d\r\n", 30);

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

    while (1)
    {
        c_keyboard();

        switch (ascii_code) /* don't ask how, when i print ascii_code as int i get weird numbers */
        {
            case -19443:                /* enter */
                printf("\r\n");
                break;

            case -19456:                /* up cursor */
                move_cursor_up();
                break;

            case -19454:                /* down cursor */
                move_cursor_down();
                break;

            case -19453:                /* left cursor */
                move_cursor_left();
                break;

            case -19452:                /* upper cursor */
                move_cursor_right();
                break;
            
            default:
                printf("%c", ascii_code);
                break;
        }
    }

end:
    for (;;);
}
