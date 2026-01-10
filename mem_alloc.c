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

	mark_free_pages();
}

void mark_free_pages(struct mmap_entry *entries, uint32_t total_entries, struct bitmap *b){
	//MM ENTRY = section/region of memory -> contains many pages -> each page represented by 1 bit basically

	for (uint32_t entry_index = 0; entry_index < total_entries; entry_index++){
		if (entries[entry_index].type != 1) continue;
		//we MUST use PAGE_SIZE as the conversion factor, otherwise we will be literally working with individual bits, which
		//doesn't really work for our page structure	
		uint32_t start_page = entries[entry_index].addr / PAGE_SIZE;
		uint32_t end_page = (entries[entry_index].addr + entries[entry_index].len) / PAGE_SIZE;
		
		//now loop from the start page to the end page and set bits accordingly
		for (uint32_t cur_page = start_page; cur_page < end_page; cur_page++){
			bitmap_clear(b, cur_page);		
		}	
	}


}

