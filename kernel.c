#include <stdint.h>
#include "gdt.h"
#include "idt.h"
#include "multiboot.h"

void kernel_main(uint32_t magic, uint32_t multiboot_addr){
//	if (magic != 0x2BADB002){
//		panic("Not booted by GRUB");
//	}

	multiboot_info_t *mbi = (multiboot_info_t *)multiboot_addr;

	gdt_init();
	//idt_init();

	volatile char *video_mem = (volatile char *)0xB8000;
	video_mem[0] = 'G';
	video_mem[1] = 0x0F;


	while(1);

}
