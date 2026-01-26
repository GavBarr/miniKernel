#include "superblock.h"
#include "inode.h"
#include "../include/block_device.h"
#include <stdint.h>
#include <stddef.h>

#define MAGIC_NUMBER 0x53465300


void superblock_init(struct Superblock *s, uint32_t disk_size){
	
	s->magic=MAGIC_NUMBER;
	s->block_size=512;
	s->inode_count=256;
	s->total_blocks=disk_size/s->block_size;

	s->inode_bitmap_block=1; //right after the superblock typically
	s->data_bitmap_block=2; 
	s->inode_table_block=3;
	
	uint32_t inode_table_blocks = (s->inode_count * sizeof(struct Inode)) / s->block_size;
	s->data_blocks_start=s->inode_table_block+inode_table_blocks;
	s->free_inodes=s->inode_count;
	s->free_blocks=s->total_blocks - s->data_blocks_start;

}

int superblock_read(struct Superblock *s, struct block_device *disk, void *buf){

	if(disk == NULL) return -1;

	int read = disk->ops->read_block(disk, 0, buf);

	if (read != 0) return -1;
	if (s->magic != MAGIC_NUMBER) return -1;	
	
	return 0;


}


int superblock_write(struct block_device *disk, struct Superblock *s){

	if(disk == NULL) return -1;

	int written = disk->ops->write_block(disk, 0, (void *)s);//fwrite((void *)s, sizeof(struct Superblock), 1, disk);
	if(written != 0) return -1;

	int flush = disk->ops->flush(disk);
	if(flush != 0) return -1;

	return 0;
}

