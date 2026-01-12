#include <stdint.h>
#include <stddef.h>
#include "gdt.h"
#include "idt.h"
#include "multiboot.h"
#include "mem_alloc.h"
#include "mm.h"
#include "paging.h"
#include "debug.h"


void kernel_main(uint32_t magic, uint32_t multiboot_addr){
//	if (magic != 0x2BADB002){
//		panic("Not booted by GRUB");
//	}

	char string[] = {'G','A','V','I','N','\1','\0'};
	//print_string(string);

	gdt_init();
	idt_init();


	multiboot_info_t *mbi = (multiboot_info_t *)multiboot_addr;
	mem_alloc_init(mbi);

	page_dir_init();
	


	while(1);

}
