
#include "stdint.h"
#include "stdio.h"
#include "io.h"
#include "filesystem/disk.h"
#include "filesystem/fat.h"
#include "filesystem/bbfs.h"
#include "memory/e820.h"
#include "memory/allocator.h"
#include "memory/heap.h"
#include "gui/gui.h"

#include "libc/vector.h"

void far* g_data = (void far*)0x00500200;
void _cdecl disk_test_write();

void _cdecl kstart_(uint16_t bootDrive)
{
    clear_screen();

    global_cursor.x = 0;
    global_cursor.y = 0;

    printf("===> Pelly Operating System - 0.2.1\r\n");
    printf("Now with keyboard cursor! %d\r\n", 30);

    DISK disk;
    if (!DISK_Initialize(&disk, bootDrive))
    {
        printf("Disk init error\r\n");
        goto end;
    }

    // x86_Disk_Write(0, 64, 0, 1, 0, 0x7c00, 0x0000);
    // disk_test_write();

    // DISK_WriteSectors(0, 19, 1, 0x7c00, 0x0000);

    DISK_ReadSectors(&disk, 19, 1, 0x7c00);

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

    BBFS_v2_params test_disk;
    bbfs_get_disk_params(test_disk.disk_label, 
                         test_disk.block_size,
                         test_disk.file_system_id,
                         0);

    /* clear_screen();

    char test_buffer[512];
    char test_buffer2[512];

    for (uint16_t x = 0; x < 512; x++) {
        test_buffer[x] = 'A';
    }

    for (uint16_t x = 0; x < 123; x++) {
        test_buffer[x*2] = 'B';
    }

    for (uint16_t x = 0; x < 512; x++) {
        putc(test_buffer[x]);
    }

    bbfs_write_block(44032, test_buffer, 512);
    bbfs_read_block(44032, test_buffer2, 512); */

    /* -------------------------------- */

    /* char buffer_b[512];

    char buffer_b[512];

    for (int x = 0; x < 512; x++) {
        buffer_b[x] = 'a';
    }

    x86_Disk_Write(1, 1, 0, 1, 0, buffer_b); */

    /* -------------------------------- */

    char test_file_name[] = "HELLO WORLD";
    char test_file_exst[] = "TXT";
    char test_data[] = "THIS IS JUST SOME RANDOM JUNK THAT WILL BE WRITTEN TO THE DISK AS [HELLO  WORLD.TXT]";

    bbfs_v3_write_file(test_file_name, test_file_exst, test_data, 1);
    
    char test_out_file[512];

    bbfs_v3_read_file(1, test_out_file);

    get_low_memory();
    get_used_memory();

    printf("Free lower memory: %d\r\n", low_memory);
    printf("Used memory: %d\r\n", used_memory);

    detect_memory();

    init_pmm();
    init_malloc();

    start_gui();

end:
    for (;;);
}
