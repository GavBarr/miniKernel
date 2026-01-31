#include <stdint.h>
#include "directory.h"
#include "file.h"
#include "inode.h"
#include "../debug/debug.h"
#include "../include/strlength.h"
#include "../include/strcompare.h"
#include "../include/memcopy.h"
#include "../include/memset.h"
#include "../include/block_device.h"
#include "../mem_alloc/heap.h"

#define FILE_TYPE 0
#define DIR_TYPE 1

static void inode_dump(struct Inode *inode);
static void debug_dump_bitmaps(struct Bitmap *inode_bitmap, struct Bitmap *block_bitmap);

int dir_add_entry(struct Inode *dir_inode, uint32_t dir_inode_num, char *name, uint32_t inode_num, uint8_t type, struct block_device *disk, struct Superblock *sb, struct Bitmap *block_bitmap){
	uint8_t block_buf[sb->block_size];
	if (dir_inode->data_blocks[0] == -1){
		uint32_t new_block = inode_allocate_block(dir_inode, block_bitmap, sb);
		if (new_block == -1) return -1;

		//OLD BUG: I wasn't writing to disk after finding the new block initially
//		if (inode_write(dir_inode, dir_inode_num, sb, disk) != 0) return -1;
//		kmemset(block_buf, 0, sb->block_size);
	}else{

		//we use data_blocks[0] for simplicity for now while in development, this MUST be enhanced at some point to look at the next available data_block entry. each block holds 512 bytes and each entry will not take up more than 1/10th of a block
//	        disk->ops->read_block(disk, dir_inode->data_blocks[0], (void *)block_buf);

	}

	        disk->ops->read_block(disk, dir_inode->data_blocks[0], (void *)block_buf);
	//if (inode_write(dir_inode, dir_inode_num, sb, disk) != 0) return -1;

	struct dir_entry entry;
	entry.inode_num = inode_num;
	entry.entry_length = sizeof(struct dir_entry) + strlength(name);
	entry.name_length = strlength(name);
	entry.file_type = type;
//	print_string(name);
//	print_int(entry.name_length);
	
       // print_string("\n\0");
	uint32_t offset = dir_inode->size;// the offset is there to go to the next section within the block that is available, so if we are in block 0 we have 512 bytes available. say there is already data within block 0, and that occupies 49bytes, we need to start then at byte 50 and onwards etc.
	//put dir_entry contents into the block + offset
	if (memcopy(block_buf + offset, &entry, sizeof(struct dir_entry)) != 0) return -1;
	//now  we add the file name that isn't fixed in size 
	if (memcopy(block_buf + offset + sizeof(struct dir_entry), name, entry.name_length) != 0) return -1;
	

	//need to write the block to disk finally
	disk->ops->write_block(disk, dir_inode->data_blocks[0], block_buf);

	dir_inode->size += entry.entry_length; //this is so we can keep track of the ACTUAL size/length of the inode entry
	
	//now write the dir_inode to disk, updating essentially
	inode_write(dir_inode, dir_inode_num, sb, disk);

	return 0;

	
}


int dir_lookup(uint32_t dir_inode_num,char *name, uint32_t *result_inode_num, struct Superblock *sb, struct block_device *disk){
	struct Inode *dir_inode = kmalloc(sizeof(struct Inode));
	inode_read(dir_inode, dir_inode_num,sb, disk);
	if (dir_inode->data_blocks[0] == -1){
		print_string("data block issue\n\0");
		kfree(dir_inode); 
		return -1;
	}

	if (dir_inode->type != DIR_TYPE){
		print_string("type issue\n\0");
		kfree(dir_inode);
		return -1;
	}	

	uint8_t block_buf[sb->block_size];
	if(disk->ops->read_block(disk, dir_inode->data_blocks[0], block_buf) != 0){
		print_string("read issue\n\0");
		kfree(dir_inode);
		return -1;
	}


	uint32_t offset = 0;
	//inode_dump(dir_inode);
	struct dir_entry *test_entry = (struct dir_entry *)(block_buf);
	char *test_entry_name = (char *)(test_entry + 1);
	while (offset < dir_inode->size){

		struct dir_entry *entry = (struct dir_entry *)(block_buf + offset);
		if (entry->name_length == strlength(name)){
			char *entry_name = (char *)(entry + 1); //name comes after the struct, thuse we need to increment by 1)
			if (strcompare(entry_name, name)){
				print_string("WE MADE IT!\n\0");
				*result_inode_num = entry->inode_num;
				kfree(dir_inode);
				return 0;

			}
		}

		offset += entry->entry_length;
	
	}

	kfree(dir_inode);
	print_string("nothing found\n\0");
	return -1;

}



int dir_remove_entry(struct Inode *inode, uint32_t inode_num, char *name, struct Superblock *sb, struct block_device *disk){

	if (inode->data_blocks[0] == -1) return -1;
        uint8_t block_buf[sb->block_size];
        disk->ops->read_block(disk, inode->data_blocks[0], block_buf);

	uint8_t offset = 0;

	while(offset < inode->size){
	
	
		struct dir_entry *entry = (struct dir_entry *)(block_buf+offset);

		if (entry->name_length == strlength(name)){
			char *entry_name = (char *)(entry + 1);

			if (strcompare(entry_name, name)){
				//remove the entry entirely
				uint32_t remaining = inode->size - (offset - entry->entry_length);
				memmove(block_buf + offset, block_buf + offset + entry->entry_length, remaining);

				inode->size -= entry->entry_length;
				disk->ops->write_block(disk, inode->data_blocks[0],block_buf);
				inode_write(inode, inode_num, sb, disk);

				return 0;
			}
			
		}
		offset += entry->entry_length;
	
	}
	return -1;
}




int dir_create(uint32_t parent_inode_num, char *name, uint32_t permissions, struct Bitmap *inode_bitmap, struct Bitmap *block_bitmap, struct Superblock *sb, struct block_device *disk){

	uint32_t inode_num;
	if (file_create(sb, &inode_num, inode_bitmap, 0777, disk) != 0) return -1;

	struct Inode *dir_inode = kmalloc(sizeof(struct Inode));
	if (inode_read(dir_inode, inode_num, sb, disk) != 0) return -1; //basically read the existing inode num
	
	

	dir_inode->type = DIR_TYPE;
	if (inode_write(dir_inode, inode_num, sb, disk) != 0) return -1;
	
	//. should point to itself, or rather it's own inode
	if (dir_add_entry(dir_inode, inode_num,".", inode_num, DIR_TYPE, disk, sb, block_bitmap) != 0) return -1;
	if (inode_read(dir_inode, inode_num, sb, disk) != 0) return -1;
	//.. will point to the parent inode that is passed
	if (dir_add_entry(dir_inode, inode_num,"..", parent_inode_num, DIR_TYPE, disk, sb , block_bitmap) != 0) return -1;
	
	struct Inode *parent_inode = kmalloc(sizeof(struct Inode));
	if (inode_read(parent_inode, parent_inode_num, sb, disk) != 0) return -1;
	if (dir_add_entry(parent_inode, parent_inode_num, name, inode_num, DIR_TYPE, disk, sb, block_bitmap) != 0) return -1;
	
	kfree(dir_inode);
	kfree(parent_inode);


	return 0;

}

int file_create_inside_dir(uint32_t parent_inode_num, char *name, uint32_t permissions, struct Bitmap *inode_bitmap, struct Bitmap *block_bitmap, struct Superblock *sb, struct block_device *disk){

        uint32_t inode_num;
        if (file_create(sb, &inode_num, inode_bitmap, permissions, disk) != 0) return -1;

        struct Inode *dir_inode = kmalloc(sizeof(struct Inode));
        if (inode_read(dir_inode, inode_num, sb, disk) != 0) return -1; //basically read the existing inode num


        dir_inode->type = FILE_TYPE;
        if (inode_write(dir_inode, inode_num, sb, disk) != 0) return -1;



        struct Inode *parent_inode = kmalloc(sizeof(struct Inode));
        if (inode_read(parent_inode, parent_inode_num, sb, disk) != 0) return -1;
        if (dir_add_entry(parent_inode, parent_inode_num, name, inode_num, DIR_TYPE, disk, sb, block_bitmap) != 0) return -1;

        kfree(dir_inode);
        kfree(parent_inode);


	//debug_dump_bitmaps(inode_bitmap, block_bitmap);

        return 0;

}







static void inode_dump(struct Inode *inode) {
    print_string("==== INODE DUMP ====\n");

    print_string("size: ");
    print_int(inode->size);
    print_string(" bytes\n");

    print_string("type: ");
    print_int(inode->type);
    print_string("\n");

    print_string("permissions: ");
    print_int(inode->permissions);
    print_string("\n");

    print_string("created_time: ");
    print_int((uint32_t)inode->created_time);
    print_string("\n");

    print_string("modified_time: ");
    print_int((uint32_t)inode->modified_time);
    print_string("\n");

    print_string("data blocks:\n");
    for (int i = 0; i < 12; i++) {
        print_string("  [");
        print_int(i);
        print_string("] = ");
        print_int(inode->data_blocks[i]);
        print_string("\n");
    }

    print_string("====================\n");
}

static void debug_dump_bitmaps(struct Bitmap *inode_bitmap, struct Bitmap *block_bitmap) {
    print_string("=== INODE BITMAP ===\n\0");
    print_string("Total inodes: \0");
    print_int(inode_bitmap->size);
    print_string("\n\0");

    print_string("Allocated inodes: \0");
    for (uint32_t i = 0; i < inode_bitmap->size && i < 32; i++) {  // Show first 32
        if (fs_bitmap_test(inode_bitmap, i)) {
            print_int(i);
            print_string(" \0");
        }
    }
    print_string("\n\0");

    print_string("First 32 bits: \0");
    for (uint32_t i = 0; i < 32 && i < inode_bitmap->size; i++) {
        print_int(fs_bitmap_test(inode_bitmap, i) ? 1 : 0);
    }
    print_string("\n\n\0");

    print_string("=== BLOCK BITMAP ===\n\0");
    print_string("Total blocks: \0");
    print_int(block_bitmap->size);
    print_string("\n\0");

    print_string("Allocated blocks: \0");
    for (uint32_t i = 0; i < block_bitmap->size && i < 32; i++) {  // Show first 32
        if (fs_bitmap_test(block_bitmap, i)) {
            print_int(i);
            print_string(" \0");
        }
    }
    print_string("\n\0");

    print_string("First 32 bits: \0");
    for (uint32_t i = 0; i < 32 && i < block_bitmap->size; i++) {
        print_int(fs_bitmap_test(block_bitmap, i) ? 1 : 0);
    }
    print_string("\n\n\0");
}

