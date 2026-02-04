#include <stdint.h>
#include <stddef.h>
#include "drivers/ramdisk.h"
#include "drivers/ide_ata_driver.h"
#include "fs/superblock.h"
#include "fs/bitmap.h"
#include "fs/inode.h"
#include "fs/file.h"
#include "fs/fs.h"
#include "fs/directory.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "include/device_manager.h"
#include "include/block_device.h"
#include "include/strlength.h"
#include "include/memcopy.h"
#include "mem_alloc/heap.h"
#include "mem_alloc/multiboot.h"
#include "mem_alloc/mem_alloc.h"
#include "mem_alloc/mm.h"
#include "paging/paging.h"
#include "debug/debug.h"
#include "kernel_shell/shell.h"

#define FILE_TYPE 0
#define DIR_TYPE 1
	

static uint32_t debug = 0;
static void test_file_ops(uint32_t disk_size, struct block_device *disk);
static void inode_dump(struct Inode *inode);
static void debug_dump_bitmaps(struct Bitmap *inode_bitmap, struct Bitmap *block_bitmap);

void kernel_main(uint32_t magic, uint32_t multiboot_addr){
//	if (magic != 0x2BADB002){
//		panic("Not booted by GRUB");
//	}


	gdt_init();
	idt_init();


	multiboot_info_t *mbi = (multiboot_info_t *)multiboot_addr;
	mem_alloc_init(mbi);

	page_dir_init();
	heap_init();
	device_manager_init();

	fs_init();
//	print_string(get_current_path());
//	int file_chan = fs_open("/test.txt",0777);
//	fs_close(file_chan);
	//char *buf = "Welcome to my kernel!";
	//fs_write(file_chan, buf, strlength(buf));


	fs_mkdir("/subdir", 0777);


	
//	print_int(get_tick_count());
	
//	struct block_device *dev = ide_init();


//	struct block_device *dev = ide_init();
//	int check = register_block_device(dev);


//	test_file_ops(dev->block_size * dev->block_count, dev);


	shell_run();

	while(1){
		
	};

}

static void test_file_ops(uint32_t disk_size, struct block_device *disk){
	fd_init();
	struct Superblock *sb = kmalloc(sizeof(struct Superblock));
	struct Bitmap *inode_bitmap = kmalloc(sizeof(struct Bitmap));
	struct Bitmap *block_bitmap = kmalloc(sizeof(struct Bitmap));
	int test_file_creation = 0;

	superblock_init(sb, disk_size);
	fs_bitmap_init(inode_bitmap, 256);
	fs_bitmap_init(block_bitmap, sb->total_blocks);

	for (uint32_t i = 0; i < sb->data_blocks_start; i++) {
    		fs_bitmap_set(block_bitmap, i);
	}
	sb->free_blocks = sb->total_blocks - sb->data_blocks_start;
	//create directory testing
	uint32_t root_inode_num;// = 1;//root exists at inode 1
	file_create(sb, &root_inode_num, inode_bitmap, 0777, disk);
	struct Inode *root_inode = kmalloc(sizeof(struct Inode));
	inode_read(root_inode, root_inode_num, sb, disk);


	root_inode->type = DIR_TYPE;
	inode_write(root_inode, root_inode_num, sb, disk);


	dir_add_entry(root_inode, root_inode_num, ".", root_inode_num, DIR_TYPE, disk, sb, block_bitmap);
	inode_read(root_inode, root_inode_num, sb, disk);
	dir_add_entry(root_inode, root_inode_num, "..", root_inode_num, DIR_TYPE, disk, sb, block_bitmap);

	dir_create(root_inode_num, "mydir", 0777, inode_bitmap, block_bitmap, sb, disk);
	
	uint32_t mydir_inode_num;
	dir_lookup(root_inode_num, "mydir", &mydir_inode_num, sb, disk);


	uint32_t path_root_inode_num;

	file_create_inside_dir(mydir_inode_num, "test.txt", 0644, inode_bitmap, block_bitmap, sb, disk);

	path_resolution(root_inode_num, "/mydir", &path_root_inode_num, sb, inode_bitmap, block_bitmap, disk);

	print_string("inode_num->\0");
	print_int(path_root_inode_num);
//	char *test_buf = "GAVIN";
//	struct Inode *test_inode = kmalloc(sizeof(struct Inode));
//	inode_read(test_inode, mydir_inode_num, sb, disk);
//	file_write(test_inode, mydir_inode_num, test_buf, 0, strlength(test_buf), block_bitmap, sb, disk);
//	char *test_buf2 = kmalloc(5);
//	file_read(test_inode, mydir_inode_num, test_buf2, 0, strlength(test_buf), sb, disk);

//	print_string(test_buf2);


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
