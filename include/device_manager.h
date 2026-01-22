#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H
#define MAX_BLOCK_DEVICES 16
#include "block_device.h"

struct device_manager{
	struct block_device *devices[MAX_BLOCK_DEVICES];
	uint32_t count;
	uint32_t initialized;

};


int device_manager_init();
int register_block_device(struct block_device *device);
struct block_device *find_device(char *name);

#endif
