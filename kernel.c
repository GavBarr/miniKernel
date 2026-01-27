#include <stdint.h>
#include <stddef.h>
#include "drivers/ramdisk.h"
#include "drivers/ide_ata_driver.h"
#include "fs/superblock.h"
#include "fs/bitmap.h"
#include "fs/inode.h"
#include "fs/file.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "include/device_manager.h"
#include "include/block_device.h"
#include "include/strlength.h"
#include "mem_alloc/heap.h"
#include "mem_alloc/multiboot.h"
#include "mem_alloc/mem_alloc.h"
#include "mem_alloc/mm.h"
#include "paging/paging.h"
#include "debug/debug.h"
#include "kernel_shell/shell.h"

static uint32_t debug = 0;
static void test_file_ops(uint32_t disk_size, struct block_device *disk);

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

	struct block_device *dev = ide_init();
	int check = register_block_device(dev);

	test_file_ops(dev->block_size * dev->block_count, dev);

	//struct block_device *dev1 = ide_init();
	//int check2 = register_block_device(dev1);
	if (debug){
		uint8_t *buffer = kmalloc(512);
		print_string(dev->name);
		print_string("\n\0");
		for (int i = 0; i < dev->block_size; i++) buffer[i]=1;
	
		int status = dev->ops->write_block(dev, 2 , buffer);
		status = dev->ops->write_block(dev, 3 , buffer);
		status = dev->ops->write_block(dev, 4 , buffer);
		status = dev->ops->write_block(dev, 500 , buffer);

		if (status != 0){
			print_string("FAILED WRITE\n\0");
		}else{
			print_string("SUCCESSFUL WRITE\n\0");
		}
		uint8_t *buffer2 = kmalloc(512);


		int status2 = dev->ops->read_block(dev, 500, buffer2);
		if (status2 != 0){
			print_string("FAILED WRITE\n\0");
		}else{
			print_string("SUCCESSFUL READ\n\0");
		}
	
		for (int i = 0; i < dev->block_size; i++){
			if(buffer2[i] == 1){
				//expected value of 1 == success
				print_string("1 \0");
				//break;
			}else{
				print_string("UNVERIFIED DATA\n\0");
				break;
			}
		}
		

		int status3 = dev->ops->flush(dev);
		print_string("\n\0");


		if(status3 == -1){
			print_string("****FLUSH DEVICE FAIL****\n\0");
		}else{
			print_string("****FLUSH DEVICE SUCCESS****");
		}

		kfree(buffer);
		kfree(buffer2);
	}

	shell_run();

	while(1){
		
	};

}

static void test_file_ops(uint32_t disk_size, struct block_device *disk){
	struct Superblock *sb = kmalloc(sizeof(struct Superblock));
	struct Bitmap *inode_bitmap = kmalloc(sizeof(struct Bitmap));
	struct Bitmap *block_bitmap = kmalloc(sizeof(struct Bitmap));


	superblock_init(sb, disk_size);
	fs_bitmap_init(inode_bitmap, 256);
	fs_bitmap_init(block_bitmap, sb->total_blocks);


	uint32_t inode_num;
	file_create(sb, &inode_num, inode_bitmap, 0777, disk);
	char *test = "Hi! This is a test for my simple filesystem...";
	uint32_t len = strlength(test);
	struct Inode *inode = kmalloc(sizeof(struct Inode));
	inode_read(inode, inode_num, sb, disk);
//	int write_status = file_write(inode, inode_num, test, 0, len, block_bitmap,  sb, disk);
//	print_int(write_status);
//	char *data = kmalloc(inode->size + 1);
//	int read_status = file_read(inode, inode_num, (void *)data, 1, inode->size, sb, disk);
//	data[inode->size] = '\0';
//	print_string(data);

//	file_delete(inode_num, sb, inode_bitmap, block_bitmap, disk);

//	kfree(sb);
//	kfree(inode_bitmap);
//	kfree(block_bitmap);
	//kfree(data);

}
