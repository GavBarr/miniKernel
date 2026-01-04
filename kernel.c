#include <stdint.h>

void kernel_main(void){
	char *video_mem = (char *)0xB8000;
	
	video_mem[0] = 'g';
	video_mem[1] = 0x0F;

}
