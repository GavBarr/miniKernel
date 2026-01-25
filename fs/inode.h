#ifndef INODE_H
#define INODE_H
#include <stdint.h>
#include "bitmap.h"
#include "superblock.h"
#include "../include/block_device.h"


struct Inode{
	uint32_t size;
	uint32_t type;
	uint32_t permissions;
	uint64_t created_time;
	uint64_t modified_time;
	uint32_t data_blocks[12];
}__attribute__((packed));

int inode_init(struct Inode *i);
int inode_write(struct Inode *i, uint32_t inode_number, struct Superblock *s, struct block_device *disk);
int inode_read(struct Inode *i, uint32_t inode_number, struct Superblock *s, struct block_device *disk);
uint32_t inode_allocate_block(struct Inode *i, struct Bitmap *b, struct Superblock *s);
int inode_free_blocks(struct Inode *i, struct Bitmap *b, struct Superblock *s);
int test_inode();



#endif

