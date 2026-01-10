#include <stdint.h>
#include "gdt.h"
#include "idt.h"
#include "multiboot.h"
#include "mem_alloc.h"
#include "mm.h"


void kernel_main(uint32_t magic, uint32_t multiboot_addr){
//	if (magic != 0x2BADB002){
//		panic("Not booted by GRUB");
//	}

	gdt_init();
	idt_init();


	multiboot_info_t *mbi = (multiboot_info_t *)multiboot_addr;
	struct mmap mem_map;

	mem_alloc_init(mbi, mem_map);

	void *page1 = alloc_page();	

	volatile char *video_mem = (volatile char *)0xB8000;
	video_mem[0] = 'G';
	video_mem[1] = 0x0F;


	while(1);

}
