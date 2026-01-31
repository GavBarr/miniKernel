#ifndef DIRECTORY_H
#define DIRECTORY_H
#include <stdint.h>
#include "inode.h"
#include "superblock.h"
#include "bitmap.h"
#include "../include/block_device.h"

struct dir_entry{
	uint32_t inode_num;
	uint16_t entry_length; //total size of the entry
	uint32_t name_length;//uint8_t name_length;
	uint8_t file_type;
//	char name[];
}__attribute__((packed));;

int dir_lookup(uint32_t dir_inode_num,char *name, uint32_t *result_inode_num, struct Superblock *sb, struct block_device *disk);
int dir_remove_entry(struct Inode *inode, uint32_t inode_num, char *name, struct Superblock *sb, struct block_device *disk);
int dir_add_entry(struct Inode *dir_inode, uint32_t dir_inode_num, char *name, uint32_t inode_num, uint8_t type, struct block_device *disk, struct Superblock *sb, struct Bitmap *block_bitmap);
int dir_create(uint32_t parent_inode, char *name, uint32_t permissions, struct Bitmap *inode_bitmap, struct Bitmap *block_bitmap, struct Superblock *sb, struct block_device *disk);
int file_create_inside_dir(uint32_t parent_inode_num, char *name, uint32_t permissions, struct Bitmap *inode_bitmap, struct Bitmap *block_bitmap, struct Superblock *sb, struct block_device *disk);

#endif
