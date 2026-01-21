#include "ramdisk.h"
#include "../include/block_device.h"
#include "../mem_alloc/heap.h"
#include "../debug/debug.h"
#include <stddef.h>
#include <stdint.h>

static struct block_device_ops ops = {
	.read_block = ram_read_block,
	.write_block = ram_write_block
};

struct block_device *ramdisk_init(){
	void *ram_data = kmalloc((512 * 1024));
        struct block_device *ram_disk = kmalloc(sizeof(struct block_device));
	
	if (ram_data == NULL || ram_disk == NULL) return NULL;

        ram_disk->name = "ram0";
        ram_disk->block_size = 512;
        ram_disk->block_count = 1024;
        ram_disk->ops = &ops;
        ram_disk->data = ram_data;

	return ram_disk;
	
}

int ram_read_block(struct block_device *dev, uint32_t block_num, void *buffer){
	if (dev == NULL) return -1;
	if (block_num >= dev->block_count) return -1;

	uint32_t offset = block_num * dev->block_size;
	uint8_t *block_start = (uint8_t *)(dev->data + offset); //we need to get set the beginning of the block and read upwards
	uint8_t *dst = (uint8_t *)buffer;	
	for (uint32_t i = 0; i < dev->block_size; i++){
		dst[i] = block_start[i];
	}

	return 0;


}

int ram_write_block(struct block_device *dev, uint32_t block_num, void *buffer){
	if (dev == NULL) return -1;
        if (block_num >= dev->block_count) return -1;

        uint32_t offset = block_num * dev->block_size;
        uint8_t *block_start = (uint8_t *)(dev->data + offset); //we need to get set the beginning of the block and read upwards
        uint8_t *src = (uint8_t *)buffer;
        for (uint32_t i = 0; i < dev->block_size; i++){
                block_start[i] = src[i];
        }

        return 0;

}
