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
	
//	walk_and_print_heap();
	shell_run();
	if (debug == 1){
		void *ptr = kmalloc(5);
		void *ptr2 = kmalloc(5);
		kfree(ptr);
		kfree(ptr2);
		walk_and_print_heap();		

	}


	


	while(1){
		
	};

}
