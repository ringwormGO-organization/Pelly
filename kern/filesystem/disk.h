
#pragma once

#include "../stdint.h"

#include "../x86.h"
#include "../stdio.h"

typedef struct {
    uint8_t id;
    uint16_t cylinders;
    uint16_t sectors;
    uint16_t heads;
} DISK;

bool DISK_Initialize(DISK* disk, uint8_t driveNumber);
bool DISK_ReadSectors(DISK* disk, uint32_t lba, uint8_t sectors, void far* dataOut);
// bool DISK_WriteSectors(DISK* disk, uint32_t lba, uint8_t sectors, void far* offset, void far* dataOut);
