#include <stdint.h>
#include <stddef.h>
#include "file.h"
#include "bitmap.h"
#include "inode.h"
#include "../debug/debug.h"
#include "../include/block_device.h"
#include "../mem_alloc/heap.h"

int file_create(struct Superblock *sb, uint32_t *inode_num, struct Bitmap *b, uint32_t permissions, struct block_device *disk){
	if (b == NULL || inode_num == NULL || sb == NULL || disk == NULL) return -1;

	uint32_t free_bit = fs_bitmap_find_free(b);
	if (free_bit == -1) return -1;
	fs_bitmap_set(b, free_bit);
	struct Inode i;// = kmalloc(sizeof(struct Inode));
	inode_init(&i);

	if (inode_write(&i, free_bit, sb, disk) != 0) return -1;
	sb->free_inodes--;
	if(superblock_write(disk, sb) != 0) return -1;
	*inode_num = free_bit;
	return 0;
	
}

int file_read(struct Inode *inode, uint32_t inode_num, void *buffer, uint32_t offset, uint32_t size, struct Superblock *sb, struct block_device *disk){

	uint32_t block_size = sb->block_size;
	uint32_t start_block = offset / block_size;
	uint32_t end_block = (offset + size - 1) / block_size;
	uint32_t bytes_read = 0;

	uint32_t index = start_block;


	if (offset >= inode->size) return 0;
        if (offset + size > inode->size) size = inode->size - offset;

	while (bytes_read < size && index < 12){
		
		uint32_t block_num = inode->data_blocks[index];
		if (block_num == (uint32_t)-1) break; //break because only empty blocks going forward
		uint8_t block_buf[block_size];
		disk->ops->read_block(disk, block_num, block_buf);
	
		uint32_t block_offset = (index == start_block) ? (offset % block_size) : 0;//uint32_t block_offset = offset % block_size;

		uint32_t chunk = block_size - block_offset; //calc bytes left to read
		if (chunk > size - bytes_read) chunk = size - bytes_read;

		uint8_t *temp_buf = (uint8_t *)buffer;
		for (int i = 0; i < chunk; i++){
			temp_buf[i + bytes_read] = block_buf[i + block_offset];	
		}
		bytes_read += chunk;
	//	offset += chunk;
		index++;
		
			
	}
	return bytes_read;


}

int file_write(struct Inode *inode, uint32_t inode_num, void *buffer, uint32_t offset, uint32_t size, struct Bitmap *b, struct Superblock *sb, struct block_device *disk){
	//block bounds
	uint32_t block_size = sb->block_size;
	uint32_t start_block = offset / block_size;
	uint32_t end_block = (offset + size - 1) / block_size;

	for (int i = start_block; i <= end_block; i++){
		if (i >= 12) return -1; //file is too big
		if (inode->data_blocks[i] == -1){
			uint32_t new_block = inode_allocate_block(inode, b, sb);
			if (new_block == -1) return -1; //bail if no space
		}
	}
	uint32_t bytes_written = 0;
	for (int i = start_block; i <= end_block; i++){
		uint32_t block_offset = (i == start_block) ? (offset % block_size) : 0;

		uint32_t bytes_to_write;
		uint32_t remaining = size - bytes_written;
		uint32_t space_in_block = block_size - block_offset;

		//basically checking to see if we are going to fill the entire block or not
		if (remaining < space_in_block){
			bytes_to_write = remaining;
		}else{
			bytes_to_write = space_in_block;
		}
		uint8_t *block_buf = (uint8_t *)kmalloc(block_size);
		if (block_buf == NULL) return -1;
		disk->ops->read_block(disk, inode->data_blocks[i], block_buf);
		for (int k = 0; k < bytes_to_write; k++){
			block_buf[block_offset + k] = ((uint8_t *)buffer)[bytes_written + k];
			
		}
		disk->ops->write_block(disk, inode->data_blocks[i], block_buf);
		bytes_written += bytes_to_write;
		kfree(block_buf);
	}

	uint32_t new_end = offset + size;
	if (new_end > inode->size){
		inode->size = new_end;
	}
	inode->modified_time = 1;//get_curtime();
	inode_write(inode, inode_num, sb, disk);

	return 0;

}

int file_delete(uint32_t inode_num, struct Superblock *sb, struct Bitmap *inode_bitmap, struct Bitmap *block_bitmap, struct block_device *disk){
	struct Inode inode;
	if (inode_read(&inode, inode_num, sb, disk) != 0) return -1;
	for (int i = 0; i < 12; i++){
		if (inode.data_blocks[i] != -1){
			fs_bitmap_clear(block_bitmap, inode.data_blocks[i]);
			inode.data_blocks[i] = -1;
		}
	}

	inode_init(&inode);
	if (inode_write(&inode, inode_num, sb, disk) != 0) return -1;
	fs_bitmap_clear(inode_bitmap, inode_num);

	sb->free_inodes++;
	superblock_write(disk, sb);

	return 0;

}


