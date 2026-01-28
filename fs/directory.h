#ifndef DIRECTORY_H
#define DIRECTORY_H
#include <stdint.h>
#include "inode.h"
#include "superblock.h"
#include "../include/block_device.h"

struct dir_entry{
	uint32_t inode_num;
	uint16_t entry_length; //total size of the entry
	uint8_t name_length;
	uint8_t file_type;
	char name[];
}__attribute__((packed));;


//int dir_lookup(struct Inode *inode, char *name, uint32_t *inode_num);
//int dir_remove_entry(struct Inode *inode, char *name);
int dir_add_entry(struct Inode *dir_inode, char *name, uint32_t inode_num, uint8_t type, struct block_device *disk, struct Superblock *sb);
//int dir_create(uint32_t parent_inode, char *name, uint32_t permissions);
#endif
