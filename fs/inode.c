#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include "inode.h"
#include "superblock.h"
#include "bitmap.h"
#include "../include/memset.h"

#define DIR_TYPE 1
#define FILE_TYPE 0

int inode_init(struct Inode *i){
	
	//init the inode struct (set defaults etc)
	i->size = 0;
        i->type = FILE_TYPE;
        i->permissions = 0666;
        i->created_time = (uint64_t)time(NULL);
        i->modified_time = (uint64_t)time(NULL);
	kmemset(i->data_blocks,-1,sizeof(i->data_blocks));	
	
	return 0;
}

//inode number is between 0-255, because we defined in the superblock that there are 256 inodes
int inode_write(struct Inode *i, uint32_t inode_number,struct Superblock *s, struct block_device *disk){

	uint32_t starting_block= s->inode_table_block;	
	uint32_t offset = (starting_block * s->block_size) + (inode_number * sizeof(struct Inode));
	
	//int result = fseek(disk, offset, SEEK_SET);
	//if (result != 0) return -1;

	int written = disk->ops->write_block(disk,offset,(void *)i);	
	if (written != 0) return -1;

	//fflush(disk);

	return 0;

}


int inode_read(struct Inode *i, uint32_t inode_number,struct Superblock *s, struct block_device *disk){

	uint32_t starting_block= s->inode_table_block;
        uint32_t offset = (starting_block * s->block_size) + (inode_number * sizeof(struct Inode));
	

	int read = disk->ops->read_block(disk, offset, (void *)i);
	if (read != 0) return -1;


	return 0;

}


uint32_t inode_allocate_block(struct Inode *i, struct Bitmap *b, struct Superblock *s){

	uint32_t data_blocks_index = 12;

	for (uint32_t index = 0; index < 12; index++){
		if (i->data_blocks[index] != (uint32_t)-1) continue;
		data_blocks_index = index; 
		break;
	}

	if (data_blocks_index == 12) return -1;
	
	uint32_t free_bit = bitmap_find_free(b);
	if (free_bit == (uint32_t)-1) return -1;

	if((bitmap_set(b, free_bit)) != 0) return -1;

	i->data_blocks[data_blocks_index] = free_bit;

	s->free_blocks--;

	return free_bit;


}


uint32_t inode_free_block(struct Inode *i, struct Bitmap *b, struct Superblock *s){


	for (uint32_t index = 0; index < 12; index ++){
		if (i->data_blocks[index] == (uint32_t)-1) continue;

		bitmap_clear(b, i->data_blocks[index]);
		i->data_blocks[index] = (uint32_t)-1;
		s->free_blocks++;
	
	}

	return 0;
}

