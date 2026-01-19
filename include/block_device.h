#ifndef BLOCK_DEVICE_H
#define BLOCK_DEVICE_H
#include <stdint.h>

struct block_device_ops{
	//called when device is registered, allocs resources, detects device params
	int (*init)(struct block_device *dev);

	//called when device is unregistered, frees resources, flush pending writes, shutdown hardware
	int (*destroy)(struct block_device *dev);

	//read exactly one block from device, copy into the buffer, blocks until complete
	int (*read_block)(struct block_device *dev, uint32_t block_num, void *buffer);

	//write exactly one block to device, copy from the buffer, blocks until complete
	int (*write_block)(struct block_device *dev, uint32_t block_num, void *buffer);

	//ensure all writes are commiteed to the device, empy hardware caches
	int (*flush)(struct block_device *dev);
};

struct block_device{
	char name[32];
	uint32_t block_size;
	uint32_t block_count;

	//storing bit level boolean properties of the device, e.g. read_only, removable, present, formatted
	uint32_t flags;

	//pointer to the block_device_ops, linkes device to the drivers functions and enables polymorphism
	struct block_device_ops *ops;

	//store driver-specific information
	void *private_data;

	//track how many users have the device open
	uint32_t open_count;

};


#endif
