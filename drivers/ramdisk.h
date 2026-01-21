#ifndef RAMDISK_H
#define RAMDISK_H
#include <stdint.h>
#include "../include/block_device.h"

struct block_device *ramdisk_init();
int ram_read_block(struct block_device *dev, uint32_t block_num, void *buffer);
int ram_write_block(struct block_device *dev, uint32_t block_num, void *buffer);



#endif

