#include <stdint.h>
#include "gdt.h"
#include "idt.h"

void kernel_main(void){
	char *video_mem = (char *)0xB8000;
	gdt_init();
	idt_init();

	video_mem[0] = 'g';
	video_mem[1] = 0x0F;

}
