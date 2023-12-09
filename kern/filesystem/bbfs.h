
#pragma once

#include "../stdint.h"
#include "disk.h"

#define MAX_BLOCK_NAME 12       /* 12-byte block names */
#define RAM_BLOCK_SIZE 512      /* maximum size of a RAM block (blck. name not counted)*/
#define REAL_BLCK_SIZE 500      /* actual size of a RAM block (-12B for blck. name)*/

typedef struct
{
    char disk_label[10];
    uint8_t block_size;
    char file_system_id[8];
} BBFS_v2_params;

typedef struct
{
    char block_name[12];
    char free_space[500];
} BBFS_v2_block_data;


void bbfs_get_disk_params(/*char disk_label[10], 
                          uint8_t block_size, 
                          char file_sys[8],
                          uint16_t device*/);

void bbfs_read_block(uint16_t block_address_src, 
                     uint16_t block_address_dest,
                     uint16_t device);
