
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
 *      write a 512 byte block from RAM. Could be
 *      written differently, but we need compatibility
 *      with OS/1 v5.4.0
 * 
 *      @param block_address_src - source of data to write
 *      
 *      @param block_address_dest - destination of where
 *                           to write the data
 * 
 *      @param num_bytes - number of bytes to write
 * 
 *      @return `BBFS_v3_error` enum value
 *
 */
int bbfs_write_block(void far* block_address_dest,
                     void far* block_address_src,
                     uint16_t num_bytes)
{
    if (_file_sys_not_recognized == true) 
    {
        return FILE_SYSTEM_NOT_RECOGNIZED;
    }

    else if (num_bytes > RAM_BLOCK_SIZE) 
    {
        return BLOCK_SIZE_LIMIT_EXCEEDED;
    }

    /* printf("BBFS: writing data from 0x%X\r\n", block_address_src);
    printf("BBFS: writing data to 0x%X\r\n", block_address_dest);
    printf("BBFS: writing %d bytes\r\n", num_bytes); */

    memcpy(block_address_dest, block_address_src, num_bytes);
    return OK;
}

/**
 *      read a 512 byte block from RAM. Could be
 *      written differently, but we need compatibility
 *      with OS/1 v5.4.0
 * 
 *      @param block_address_src - source of data to read
 *      
 *      @param buffer - destination of where
 *                           to write the read data
 * 
 *      @param num_bytes - number of bytes to read
 * 
 *      @return `BBFS_v3_error` enum value
 *
 */
int bbfs_read_block(void far* block_address_src,
                     uint8_t buffer[512],
                     uint16_t num_bytes)
{

    if (_file_sys_not_recognized == true) 
    {
        return FILE_SYSTEM_NOT_RECOGNIZED;
    }

    else if (num_bytes > RAM_BLOCK_SIZE) 
    {
        return BLOCK_SIZE_LIMIT_EXCEEDED;
    }

    /* printf("BBFS: reading data from 0x%X\r\n", block_address_src);
    printf("BBFS: writing %d bytes\r\n", num_bytes); */

    memcpy(buffer, block_address_src, num_bytes);
    return OK;
}

/**
 * IMPORTANT:
 * I swear to God if I have to touch this file system
 * one more time I will go sign myself in to a insane
 * asylum. None of this works. For some reason the
 * read function doesn't put the first few bytes in
 * the buffer, but it will add few bytes to the end
 * of the buffer. Over the last few days that I had
 * to work with this I have lost my sanity. To anyone
 * whom it my concern: I think that the issue is with
 * strcmp and data_buffer, if you find a way to fix it
 * please do submit a PR.
 * 
 * - stjepanbm1
*/

/**
 * just a bare bones function for 
 * writing a file. Only for BBFS v3
*/

void _cdecl bbfs_v3_write_file(char file_name[], char file_exst[], char data[], int file_id) {
    char data_buffer[512];

    // copy the file name
    for (int x = 0; x <= 12; x++)
        data_buffer[x] = file_name[x];

    // copy the file exst.
    for (int x = 13; x <= 16; x++)
        data_buffer[x] = file_exst[x-13];

    // copy data to the data_buffer
    for (int x = 16; x <= 496; x++)
        data_buffer[x] = data[x-16];

    // write to disk
    // TODO: fix the junk writing after the data[] is written.
    /* printf("BBFS: writing file [%s.%s] to disk...\r\n", file_name, file_exst); */
    x86_Disk_Write(1, 1, 0, file_id, 0, data_buffer);

    // finished
    /* printf("BBFS: finished writing file [%s.%s] to the disk.\r\n", file_name, file_exst); */
}


/**
 * just a bare bones function for 
 * readining a file. Only for BBFS v3
*/
void _cdecl bbfs_v3_read_file(int file_id, char data[])
{
    char data_buffer[512];

    // printf("BBFS: reading file [%d] from disk...\r\n", file_id);

    x86_Disk_Read(1, 1, 0, file_id, 0, data_buffer);

    // finished
    /* printf("BBFS: file [%d] read.\r\n", file_id); */
}

/**
 * Search for files
 * 
 * NOTE: NOT WORKING! I think the issue is with strcmp
 * @param file_name name of file to be searched
 * @return file sector
*/
int bbfs_v3_search_for_file(char file_name[])
{
    char data_buffer[512];
    char tmp_file_name[13];
    bool found_file = false;
    int file_id = 1;

    while (!found_file)
    {
        x86_Disk_Read(1, 1, 0, file_id, 0, data_buffer);

        for (int x = 0; x <= 12; x++)
            tmp_file_name[x] = data_buffer[x];

        if (strcmp(tmp_file_name, file_name) == 0) {
            found_file = true;
            printf("BBFS: file [%d] checked. File [%s] was found there.\r\n", file_id, file_name);
            return file_id;

        }
        
        if (file_id >= 16) {
            printf("BBFS: checked all files and file [%s] was not found.\r\n", file_name);
            return -1;
        }

        file_id++;

    }

}
