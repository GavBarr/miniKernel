#include <stdint.h>
#include <stddef.h>
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "mem_alloc/heap.h"
#include "mem_alloc/multiboot.h"
#include "mem_alloc/mem_alloc.h"
#include "mem_alloc/mm.h"
#include "paging/paging.h"
#include "debug/debug.h"

static uint32_t debug = 1;

void kernel_main(uint32_t magic, uint32_t multiboot_addr){
//	if (magic != 0x2BADB002){
//		panic("Not booted by GRUB");
//	}

	print_string("GOOSEOS:\n\0");

	gdt_init();
	idt_init();


	multiboot_info_t *mbi = (multiboot_info_t *)multiboot_addr;
	mem_alloc_init(mbi);

	page_dir_init();
	heap_init();
	if (debug == 1){
		void *ptr = kmalloc(10);
		struct block_header *block = (struct block_header *)ptr;
		print_string("\n\0");
		print_string("block_addr -> \0");
		print_pointer(ptr);
		print_string("\n\0");
		print_string("block_addr.size -> \0");
		print_int((uint32_t)block->size);
		print_string("\n\0");
	        print_string("block_addr.free_flag -> \0");
	        print_int((uint32_t)block->free_flag);
		print_string("\n\0");
	        print_string("block_addr.magic -> \0");
	        print_pointer((void *)block->magic);
		print_string("\n\0");
		print_string("\n\0");
		print_string("-------------------------------\n\0");
		void *ptr2 = kmalloc(10);
                struct block_header *block2 = (struct block_header *)ptr2;
                print_string("\n\0");
                print_string("block_addr -> \0");
                print_pointer(ptr2);
                print_string("\n\0");
                print_string("block_addr.size -> \0");
                print_int((uint32_t)block2->size);
                print_string("\n\0");
                print_string("block_addr.free_flag -> \0");
                print_int((uint32_t)block2->free_flag);
                print_string("\n\0");
                print_string("block_addr.magic -> \0");
                print_pointer((void *)block2->magic);
	}


	


	while(1);

}
