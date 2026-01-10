#ifndef MEM_ALLOC_H
#define MEM_ALLOC_H
#include "mm.h"
#include "multiboot.h"
#include "bitmap.h"
#include <stdint.h>



void mem_alloc_init(multiboot_info_t *mbi, struct mmap mem_map);
void mark_free_pages(struct mmap_entry *entries, uint32_t total_entries, struct bitmap *b);



#endif
