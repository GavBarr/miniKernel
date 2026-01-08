#include "keyboard.h"

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


void keyboard_handler(void){

        uint8_t scancode = inb(0x60);

        if (scancode >= 0x80){
                return;
        }

        char ascii = scancode_to_ascii[scancode];

        if (ascii != 0){

                static int cursor_pos = 0;
                volatile char *video_cursor = (volatile char *)0xB8000;

                video_cursor[cursor_pos * 2] = ascii;
                video_cursor[cursor_pos * 2 + 1] = 0x0F;

                cursor_pos++;

                if (cursor_pos >= 80 * 25){
                        cursor_pos = 0;

                }


        }
}

static inline uint8_t inb(uint16_t port){
        uint8_t return_val;

        __asm__ volatile ("inb %1, %0"
                         : "=a"(return_val)
                         : "Nd"(port)
                         );

        return return_val;
}

