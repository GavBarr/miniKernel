#ifndef DEBUG_H
#define DEBUG_H
#include <stdint.h>

void print_string(char string[]);
void print_pointer(void *ptr);
void print_int(uint32_t n);
void display_registers(void);
void trace_start(void);
void trace_stop(void);

#endif
