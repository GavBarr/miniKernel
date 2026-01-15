#ifndef HEAP_H
#define HEAP_H
#include <stdint.h>

//12 bytes
struct block_header{
	uint32_t size; 
	uint32_t free_flag;
	uint32_t magic;
}__attribute__((packed));

void heap_init();
void *kmalloc(uint32_t size);
void kfree(void *addr);



#endif
