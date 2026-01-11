#ifndef MEM_ALLOC_H
#define MEM_ALLOC_H
#include "mm.h"
#include "multiboot.h"
#include "bitmap.h"
#include <stdint.h>



void mem_alloc_init(multiboot_info_t *mbi, struct mmap mem_map);
void *kmalloc_page();
uint32_t kfree_page(void *addr);



#endif
