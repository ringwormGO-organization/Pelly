
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
void bbfs_get_disk_params(/*char disk_label[10], 
                          uint8_t block_size,
                          char file_sys[8],
                          uint16_t device*/)
{
    uint8_t buffer[512];
    x86_Disk_Read(0, 0, 1, 0, 1, buffer);
    printf("\r\nBBFS:\r\n   ");

    for (uint16_t x = 3; x < 512; x++)
        putc(buffer[x]);
}

/**
 *  bbfs_read_block:
 *      read a 512 byte block from RAM. Could be
 *      written differently, but we need compatibility
 *      with OS/1 v5.4.0
 * 
 *  paremeters:
 *      block_address_src - addres from the source
 *                          of the block
 * 
 *      block_address_dest - destination of where 
 *                           to copy the data to
 */
void bbfs_read_block(uint16_t block_address_src, 
                     uint16_t block_address_dest,
                     uint16_t device)
{}
