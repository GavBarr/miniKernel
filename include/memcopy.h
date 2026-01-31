#ifndef MEMCOPY_H
#define MEMCOPY_H
#include <stdint.h>

int memcopy(void *destination, void *source, uint32_t length);
int memmove(void *destination, void *source, uint32_t length);

#endif
