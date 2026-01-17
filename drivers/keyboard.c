#include "../debug/debug.h"
#include "keyboard.h"
#include <stddef.h>
const char scancode_to_ascii[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*',
    0,   // Alt
    ' ', // Space
    0,   // Caps lock
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F1-F10
    0,   // Num lock
    0,   // Scroll lock
    0,   // Home
    0,   // Up arrow
    0,   // Page up
    '-',
    0,   // Left arrow
    0,
    0,   // Right arrow
    '+',
    0,   // End
    0,   // Down arrow
    0,   // Page down
    0,   // Insert
    0,   // Delete
    0, 0, 0,
    0, 0, // F11, F12
    0,   // All other keys undefined
};

static uint32_t write_index = 0;
static uint32_t read_index = 0;
static char buffer[256];

void keyboard_handler(void){

        uint8_t scancode = inb(0x60);
        if (scancode >= 0x80){
                return;
        }


        char ascii = scancode_to_ascii[scancode];

        if (ascii != 0){
		uint32_t next_write = (write_index + 1) % 256;
		if (next_write != read_index){
			buffer[write_index] = ascii;
			write_index = next_write;
		}
        }
}

char keyboard_getchar(void){
	

	while (read_index == write_index)
	{
		__asm__ volatile("hlt");
	}
	char character = buffer[read_index];
	read_index = (read_index + 1) % 256;
	return character;

}

char *get_keyboard_buffer(){
	return buffer;
}

void clear_keyboard_buffer(){
	for (uint32_t i = 0; i < 256; i++){
		buffer[i] = 0;
	}
	
	write_index=0;
	read_index=0;
}

static inline uint8_t inb(uint16_t port){
        uint8_t return_val;

        __asm__ volatile ("inb %1, %0"
                         : "=a"(return_val)
                         : "Nd"(port)
                         );

        return return_val;
}

