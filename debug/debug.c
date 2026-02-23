#include "debug.h"
#define TEXT_COLOR 0x0B
#include <stdint.h>

static volatile char *video_mem = (volatile char *)0xB8000;
static uint32_t current = 0;

void print_string(char string[]){
	uint32_t index = 0;
	while (string[index] != '\0'){
		if (string[index] == '\n'){
			current = (current / 160 + 1) * 160;
			index++;
			continue;
		}		
		video_mem[current] = string[index];
		video_mem[current+1] = TEXT_COLOR;
		index++;
		current+=2;
	}

}

void display_registers(void){
	uint32_t eax;
   	uint32_t ebx;
        uint32_t ecx;
    	uint32_t edx;
	uint32_t esi;
	uint32_t edi;
	uint32_t ebp;
	uint32_t esp;
    
    	uint32_t cs;
	uint32_t ds;
   	uint32_t ss;
    	uint32_t es;
    	uint32_t fs;
    	uint32_t gs;
    
    	uint32_t eip;
    	uint32_t eflags;
    
    	uint32_t cr0;
    	uint32_t cr2;
    	uint32_t cr3;
    	uint32_t cr4;

	__asm__ volatile ("movl %%eax, %0" : "=r" (eax));
	__asm__ volatile ("movl %%ebx, %0" : "=r" (ebx));
	__asm__ volatile ("movl %%ecx, %0" : "=r" (ecx));
	__asm__ volatile ("movl %%edx, %0" : "=r" (edx));
	__asm__ volatile ("movl %%esi, %0" : "=r" (esi));
	__asm__ volatile ("movl %%edi, %0" : "=r" (edi));
	__asm__ volatile ("movl %%ebp, %0" : "=r" (ebp));
	__asm__ volatile ("movl %%esp, %0" : "=r" (esp));
	__asm__ volatile ("movl %%cs, %0" : "=r" (cs));
	__asm__ volatile ("movl %%ds, %0" : "=r" (ds));
	__asm__ volatile ("movl %%ss, %0" : "=r" (ss));
	__asm__ volatile ("movl %%es, %0" : "=r" (es));
	__asm__ volatile ("movl %%fs, %0" : "=r" (fs));
	__asm__ volatile ("movl %%gs, %0" : "=r" (gs));
	__asm__ volatile ("movl %%cr0, %0" : "=r" (cr0));
	__asm__ volatile ("movl %%cr2, %0" : "=r" (cr2));
	__asm__ volatile ("movl %%cr3, %0" : "=r" (cr3));
	__asm__ volatile ("movl %%cr4, %0" : "=r" (cr4));
	
	print_string("eax->\0");
	print_int(eax);
	print_string(" \0");

	print_string("ebx->\0");
        print_int(ebx);
        print_string(" \0");

	print_string("ecx->\0");
        print_int(ecx);
        print_string(" \0");

        print_string("edx->\0");
        print_int(edx);
        print_string(" \0");

	print_string("esi->\0");
        print_int(esi);
        print_string(" \0");

        print_string("edi->\0");
        print_int(edi);
        print_string(" \0");

	print_string("ebp->\0");
        print_int(ebp);
        print_string(" \0");

        print_string("esp->\0");
        print_int(esp);
        print_string("\n\0");

	print_string("cs->\0");
        print_int(cs);
        print_string(" \0");

        print_string("ds->\0");
        print_int(ds);
        print_string(" \0");

        print_string("ss->\0");
        print_int(ss);
        print_string(" \0");

        print_string("es->\0");
        print_int(es);
        print_string(" \0");

        print_string("fs->\0");
        print_int(fs);
        print_string(" \0");

        print_string("gs->\0");
        print_int(gs);
        print_string("\n\0");

        print_string("cr0->\0");
        print_int(cr0);
        print_string(" \0");

        print_string("cr2->\0");
        print_int(cr2);
        print_string(" \0");

	print_string("cr3->\0");
        print_int(cr3);
        print_string(" \0");

        print_string("cr4->\0");
        print_int(cr4);
        print_string(" \0");

}


void print_pointer(void *ptr){
	uint32_t value = (uint32_t)ptr;

	char hex_chars[] = "0123456789ABCDEF";
	char buffer[11];

	buffer[0] = '0';
	buffer[1] = 'x';

	for (int i = 0; i < 8; i++){
    		buffer[9 - i] = hex_chars[value & 0xF];  // Fill from right to left
		    value >>= 4;
	}		
	buffer[10] = '\0';
	print_string(buffer);
	current+=2; //give space in between pointer vals

	
}






void print_int(uint32_t n) {

    char buffer[11]; // max 10 digits for 32-bit + null
    buffer[10] = '\0';

    int i = 9; // start from the end
    if (n == 0) {
        buffer[i] = '0';
        i--;
    } else {
        while (n > 0 && i >= 0) {
            buffer[i] = '0' + (n % 10);
            n /= 10;
            i--;
        }
    }

    // print the string starting from first non-empty char
    print_string(&buffer[i + 1]);
}




