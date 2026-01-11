#include "debug.h"
#define TEXT_COLOR 0x0A
#include <stdint.h>

static volatile char *video_mem = (volatile char *)0xB8000;
static uint32_t current = 0;

void print_string(char string[]){
	
	uint32_t index = 0;
	while (string[index] != '\0'){		
		video_mem[current] = string[index];
		video_mem[current+1] = TEXT_COLOR;
		index++;
		current+=2;
	}

}


void print_anything(void *anything){



}


