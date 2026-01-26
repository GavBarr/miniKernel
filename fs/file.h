#ifndef FILE_H
#define FILE_H
#include <stdint.h>
#include "inode.h"
#include "bitmap.h"
#include "superblock.h"
#include "../include/block_device.h"

int file_create(struct Superblock *sb, uint32_t *inode_num, struct Bitmap *b, uint32_t permissions, struct block_device *disk);
//void file_open();
int file_read(struct Inode *inode, uint32_t inode_num, void *buffer, uint32_t offset, uint32_t size, struct Superblock *sb, struct block_device *disk);
int file_write(struct Inode *inode, uint32_t inode_num, void *buffer, uint32_t offset, uint32_t size, struct Bitmap *b, struct Superblock *sb, struct block_device *disk);
//void file_close();
int file_delete(uint32_t inode_num);


#endif
