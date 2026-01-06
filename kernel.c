#include <stdint.h>
#include "gdt.h"
#include "idt.h"

void kernel_main(void){


	gdt_init();
	idt_init();

	volatile char *video_mem = (volatile char *)0xB8000;
	video_mem[0] = 'G';
	video_mem[1] = 0x0F;


	while(1);

}
