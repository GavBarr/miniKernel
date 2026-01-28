#include <stdint.h>
#include "directory.h"
#include "inode.h"
#include "../debug/debug.h"
#include "../include/strlength.h"
#include "../include/memcopy.h"
#include "../include/block_device.h"
#include "../mem_alloc/heap.h"

int dir_add_entry(struct Inode *dir_inode, char *name, uint32_t inode_num, uint8_t type, struct block_device *disk, struct Superblock *sb){

	struct dir_entry entry;
	entry.inode_num = inode_num;
	entry.entry_length = sizeof(struct dir_entry) + strlength(name);
	entry.name_length = strlength(name);
	entry.file_type = type;
	
	uint8_t *block_buf = kmalloc(512);
	//we use data_blocks[0] for simplicity for now while in development, this MUST be enhanced at some point to look at the next available data_block entry. each block holds 512 bytes and each entry will not take up more than 1/10th of a block
	disk->ops->read_block(disk, dir_inode->data_blocks[0], (void *)block_buf);
	uint32_t offset = dir_inode->size;

	//put dir_entry contents into the block + offset
	if (memcopy(block_buf + offset, &entry, sizeof(struct dir_entry)) != 0) return -1;
	//now  we add the file name that isn't fixed in size 
	if (memcopy(block_buf + offset + sizeof(struct dir_entry), name, entry.name_length) != 0) return -1;
	

	//need to write the block to disk finally
	disk->ops->write_block(disk, dir_inode->data_blocks[0], block_buf);

	dir_inode->size += entry.entry_length; //this is so we can keep track of the ACTUAL size/length of the inode entry
	
	//now write the dir_inode to disk, updating essentially
	inode_write(dir_inode, inode_num, sb, disk);
	kfree(block_buf);

	return 0;

	
}
