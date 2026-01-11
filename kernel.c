#include <stdint.h>
#include <stddef.h>
#include "gdt.h"
#include "idt.h"
#include "multiboot.h"
#include "mem_alloc.h"
#include "mm.h"
#include "debug.h"


void kernel_main(uint32_t magic, uint32_t multiboot_addr){
//	if (magic != 0x2BADB002){
//		panic("Not booted by GRUB");
//	}

	char string[] = {'G','A','V','I','N','\1','\0'};

	gdt_init();
	idt_init();


	multiboot_info_t *mbi = (multiboot_info_t *)multiboot_addr;
	mem_alloc_init(mbi);


	void *page1 = kmalloc_page();
	void *page2 = kmalloc_page();
	kfree_page(page1);
	page1 = kmalloc_page();	





	while(1);

}
