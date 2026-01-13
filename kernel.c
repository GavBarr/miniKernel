#include <stdint.h>
#include <stddef.h>
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "mem_alloc/multiboot.h"
#include "mem_alloc/mem_alloc.h"
#include "mem_alloc/mm.h"
#include "paging/paging.h"
#include "debug/debug.h"


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

	uint32_t *data = (uint32_t *)0x00200000;
	*data = 0xFEEDBEEF;

	print_pointer((void *)data);
	print_string("PASSED");
	


	while(1);

}
