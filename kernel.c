#include <stdint.h>
#include <stddef.h>
#include "drivers/ramdisk.h"
#include "drivers/ide_ata_driver.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "include/device_manager.h"
#include "include/block_device.h"
#include "mem_alloc/heap.h"
#include "mem_alloc/multiboot.h"
#include "mem_alloc/mem_alloc.h"
#include "mem_alloc/mm.h"
#include "paging/paging.h"
#include "debug/debug.h"
#include "kernel_shell/shell.h"

static uint32_t debug = 0;

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

	struct block_device *dev = ramdisk_init();
	int check = register_block_device(dev);

	struct block_device *dev1 = ide_init();
	int check2 = register_block_device(dev1);
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
