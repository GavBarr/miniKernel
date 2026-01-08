#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <stdint.h>


void keyboard_handler(void);
static inline uint8_t inb(uint16_t port);


#endif
