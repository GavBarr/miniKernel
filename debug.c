#include "debug.h"
#define TEXT_COLOR 0x0A
#include <stdint.h>

static volatile char *video_mem = (volatile char *)0xB8000;
static uint32_t current = 0;

void print_string(char string[]){
	//uint32_t string_len = 0;
	//while (string[string_len] != '\1' && string[string_len] != '\0'){
	//	string_len++;
	//}


	uint32_t index = 0;
	while (string[index] != '\0'){
		if (string[index] == '\1'){
			current += 160;//(160 - string_len);
			index++;
			continue;
		}		
		video_mem[current] = string[index];
		video_mem[current+1] = TEXT_COLOR;
		index++;
		current+=2;
	}

}


void print_pointer(void *ptr){
	uint32_t value = (uint32_t)ptr;

	char hex_chars[] = "0123456789ABCDEF";
	char buffer[11];

	buffer[0] = '0';
	buffer[1] = 'x';

	for (int i = 7; i >= 0; i--){
		buffer[2 + (7 - i)] = hex_chars[value & 0xF];
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




