#ifndef STDLIB_H
#define STDLIB_H
#include <stdint.h>

uint32_t strcompare(const char *first, const char *second);
uint32_t strlength(const char *string);
int memcopy(void *destination, void *source, uint32_t length);
int memmove(void *destination, void *source, uint32_t length);
int kmemset(void *ptr, uint32_t val, uint32_t size);


#endif
