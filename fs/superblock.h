#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H
#include <stdint.h>
#include "../include/block_device.h"

struct Superblock {
        uint32_t magic;
        uint32_t block_size;
        uint32_t total_blocks;
        uint32_t inode_count;

        uint32_t inode_bitmap_block;
        uint32_t data_bitmap_block;
        uint32_t inode_table_block;
        uint32_t data_blocks_start;

        uint32_t free_inodes;
        uint32_t free_blocks;

};

void superblock_init(struct Superblock *s, uint32_t disksize);

int superblock_read(struct Superblock *s, struct block_device *disk, void *buf);

int superblock_write(struct block_device *disk, struct Superblock *s);

int print_validate_superblock(void);


#endif
