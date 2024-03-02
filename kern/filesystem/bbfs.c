
#include "bbfs.h"

/**
 *          Bad Block File System v2
 *          ------------------------
 *      Bad Block File System v2 or BBFS v2 is
 *  the second edition of a bad file system. As the
 *  name says it uses RAM blocks for file storage.
 *  For now it is only in RAM because if we try to
 *  write data to the disk FAT12 starts complaining.
 * 
 *  Also we only support BBFS v2.
 * 
 *  Oh btw: if you ever question why does every
 *  function end with (function_name)_end label
 *  it is because I like using goto statements!
 * 
 *  TODO: tell FAT to stfu and write to disk.
 */

/**
 *  bbfs_get_disk_params:
 *      read the first sector of the disk
 *      to get parameters.
 * 
 *  paremeters:
 *      disk_label[10] - 10 bytes for the disk
 *                       label (starting from the 
 *                              3rd byte)
 * 
 *      block_size  -   1 byte, practically useless
 * 
 *      file_sys[8] -   file system id, used to check
 *                      if either BBFS v1 or BBFS v2
 */
void bbfs_get_disk_params(char disk_label[10], 
                          uint8_t block_size,
                          char file_sys[8],
                          uint16_t device)
{
    uint8_t buffer[512];
    x86_Disk_Read(device, 0, 1, 0, 1, buffer);

    printf("\r\nBBFS:");

    for (uint16_t x = 15; x <= 23-1; x++)
        file_sys[x-15]= buffer[x];

    if (strcmp(file_sys, "BBFS V02") != 0) {
        printf(" file system not recognized.\r\n");
        _file_sys_not_recognized = true;
        goto _end_bbfs_d_params;
    }

    for (uint16_t x = 3; x <= 13; x++)
        disk_label[x-3] = buffer[x];
    
    block_size = buffer[14];

    printf("\r\n");
    printf("    DISK LABEL (10 characters): %s\r\n", disk_label);
    printf("    BOOTABLE (1 - yes | 0 - no): %d\r\n", block_size);
    printf("    FILE SYSTEM ID: %s\r\n", file_sys);

_end_bbfs_d_params:
    printf("BBFS: finished.\r\n");
}

/**
 *  bbfs_write_block:
 *      write a 512 byte block from RAM. Could be
 *      written differently, but we need compatibility
 *      with OS/1 v5.4.0
 * 
 *  paremeters:
 * 
 *      block_address_src - source of data to write
 *      
 *      block_address_dest - destination of where
 *                           to write the data
 * 
 *      num_bytes - number of bytes to write
 *
 */
void bbfs_write_block(void far* block_address_dest,
                     void far* block_address_src,
                     uint16_t num_bytes)
{
    if (_file_sys_not_recognized == true) {
        printf("BBFS: cannot read RAM blocks. File system not recognized.\r\n");
        goto bbfs_write_block_end;
    }
    else if (num_bytes > RAM_BLOCK_SIZE) {
        printf("BBFS: block size limit exceeded. %dB out of 512B maximum.\r\n", num_bytes);
        goto bbfs_write_block_end;
    }

    printf("BBFS: writing data from 0x%X\r\n", block_address_src);
    printf("BBFS: writing data to 0x%X\r\n", block_address_dest);
    printf("BBFS: writing %d bytes\r\n", num_bytes);

    memcpy(block_address_dest, block_address_src, num_bytes);

bbfs_write_block_end:
    printf("BBFS: finished.\r\n");
}

void bbfs_read_block(void far* block_address_src,
                     uint8_t buffer[512],
                     uint16_t num_bytes)
{

    if (_file_sys_not_recognized == true) {
        printf("BBFS: cannot read RAM blocks. File system not recognized.\r\n");
        goto bbfs_read_block_end;
    }
    else if (num_bytes > RAM_BLOCK_SIZE) {
        printf("BBFS: block size limit exceeded. %dB out of 512B maximum.\r\n", num_bytes);
        goto bbfs_read_block_end;
    }

    printf("BBFS: reading data from 0x%X\r\n", block_address_src);
    printf("BBFS: writing %d bytes\r\n", num_bytes);

    memcpy(buffer, block_address_src, num_bytes);

bbfs_read_block_end:
    printf("BBFS: finished.\r\n");
}

/**
 * just a bare bones function for 
 * writing a file. Only for BBFS v3
*/

void bbfs_v3_write_file() {
    BBFS_v3_file_header test_file;
    char data_buffer[512];

    memset(data_buffer, 0, 512);

    for (int x = 0; x < 512; x++)
        data_buffer[x] = ' ';

    // Clear data_buffer
    //memset(data_buffer, 0, 512);
    strcpy((char *)test_file.file_name, "HELLO WORLD \0");
    strcpy((char *)test_file.file_exst, "TXT\0");

    test_file.is_file_exe = 1;
    test_file.is_sys_file = 0;

    strcpy((char *)test_file.day, "02\0");
    strcpy((char *)test_file.month, "03\0");
    strcpy((char *)test_file.year, "2024\0");

    // Copy the file header to data_buffer

    // TODO: change the loops to the strncpy

    // Copy the file_name
    for (int x = 3; x <= 15; x++)
        data_buffer[x+3] = test_file.file_name;

    // Copy the file exst.
    for (int x = 15; x <= 18; x++)
        data_buffer[x+15] = test_file.file_exst;

    // Is file exe?
    for (int x = 18; x <= 19; x++)
        data_buffer[x+18] = test_file.is_file_exe;

    // Is sys. file?
    for (int x = 19; x <= 20; x++)
        data_buffer[x+19] = test_file.is_sys_file;

    // day
    for (int x = 20; x <= 22; x++)
        data_buffer[x+20] = test_file.day;

    // month
    for (int x = 22; x <= 24; x++)
        data_buffer[x+22] = test_file.month;

    // year
    for (int x = 24; x <= 28; x++)
        data_buffer[x+24] = test_file.year;

}


/**
 * just a bare bones function for 
 * readining a file. Only for BBFS v3
*/
void bbfs_v3_read_file()
{
    printf("WIP");
}
