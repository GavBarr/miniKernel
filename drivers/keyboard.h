#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <stdint.h>


void keyboard_handler(void);
char keyboard_getchar(void);
char *get_keyboard_buffer(void);
void clear_keyboard_buffer(void);
static inline uint8_t inb(uint16_t port);


#endif
