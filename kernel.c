#include <stdint.h>
#include <stddef.h>
#include "drivers/ramdisk.h"
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
	
	uint8_t *buffer = kmalloc(512);
	for (int i = 0; i < dev->block_size; i++) buffer[i]=1;
	
	dev->ops->write_block(dev, 0 , buffer);

	uint8_t *buffer2 = kmalloc(512);
	dev->ops->read_block(dev, 0, buffer2);

//	for (int i = 0; i < dev->block_size; i++) print_int(buffer2[i]);

	kfree(buffer);
	kfree(buffer2);
	shell_run();

	while(1){
		
	};

}
