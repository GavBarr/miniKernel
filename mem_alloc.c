#include "mem_alloc.h"
#include "mm.h"
#include "multiboot.h"
#include "bitmap.h"
#include <stdint.h>

#define PAGE_SIZE 4096

void mem_alloc_init(multiboot_info_t *mbi, struct mmap mem_map){
	copy_mmap(mbi, mem_map);
	uint32_t total_entries = get_total_entries(mem_map);
	struct mmap_entry entries[total_entries];
	create_mmap_entries(mem_map, entries);

	uint64_t highest_addr = find_highest_address(entries, total_entries);

	uint64_t total_pages = highest_addr / PAGE_SIZE;
	extern uint32_t kernel_end; //this will be passed through my .asm	

	uint8_t *bitmap_data = (uint8_t *)kernel_end;
	struct bitmap *b;
	bitmap_init(b, bitmap_data, total_pages);
}


