#include "mem_alloc.h"
#include "mm.h"
#include "multiboot.h"
#include "bitmap.h"
#include <stdint.h>
#include <stddef.h>
#include "../debug/debug.h"


#define PAGE_SIZE 4096

//static struct mmap mem_map;i
static struct bitmap b;
static void mark_bitmap_used_pages(struct bitmap *b);
static void mark_kernel_used_pages(struct bitmap *b, uint32_t kernel_start, uint32_t kernel_end);
static void mark_free_pages(struct mmap_entry *entries, uint32_t total_entries, struct bitmap *b);

void mem_alloc_init(multiboot_info_t *mbi){
	copy_mmap(mbi);
	uint32_t total_entries = get_total_entries();
	struct mmap_entry entries[total_entries];
	create_mmap_entries(entries);

	uint64_t highest_addr = find_highest_address(entries, total_entries);

	uint64_t total_pages = highest_addr / PAGE_SIZE;
	//ALWAYS declare external symbol references as CHAR, otherwise you can get weird bugs. Convert if needed afterwards
	extern char kernel_start; //this will be passed through my .asm
	extern char kernel_end; //this will be passed through my .asm
	
	uint32_t start_kernel = (uint32_t)&kernel_start;
	uint32_t end_kernel = (uint32_t)&kernel_end;


	uint8_t *bitmap_data = (uint8_t *)end_kernel;
	bitmap_init(&b, bitmap_data, total_pages);
	//char test[5] = {'t','e','s','t','\0'};
	//print_string(test);

	mark_free_pages(entries, total_entries, &b);
	mark_kernel_used_pages(&b, start_kernel, end_kernel);
	mark_bitmap_used_pages(&b);

	for (uint32_t page = 0; page < 256; page++){

		bitmap_set(&b, page);
	}



}

static void mark_free_pages(struct mmap_entry *entries, uint32_t total_entries, struct bitmap *b){
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

//mark the section where the kernel lives, so we avoid overwritting it
static void mark_kernel_used_pages(struct bitmap *b, uint32_t kernel_start, uint32_t kernel_end){

	//define where the kernel starts and ends, in relativity to pages, which is why we need to convert using PAGE_SIZE
	uint32_t start_page = kernel_start / PAGE_SIZE;
	uint32_t end_page = (kernel_end + PAGE_SIZE - 1) / PAGE_SIZE;//this is
	//print_int(end_page);
	//print_pointer(end_page);


	for (uint32_t cur_page = start_page; cur_page < end_page; cur_page++){
		bitmap_set(b, cur_page);
	
	}
}

static void mark_bitmap_used_pages(struct bitmap *b){

	uint32_t bitmap_size_bytes = (b->size + 7) / 8;
        uint32_t start_page = (uint32_t)b->data / PAGE_SIZE;
        uint32_t end_page = (((uint32_t)b->data + bitmap_size_bytes) + PAGE_SIZE - 1) / PAGE_SIZE; 
	
        for (uint32_t cur_page = start_page; cur_page < end_page; cur_page++){
                bitmap_set(b, cur_page);

        }
}


void *kmalloc_page(){


	uint32_t free_page = bitmap_find_free(&b);

	if (free_page == (uint32_t)-1) return NULL;
	bitmap_set(&b, free_page);
	uint32_t addr = free_page * PAGE_SIZE;


	return (void *)addr;

	

}

uint32_t kfree_page(void *addr){

	uint32_t page_number = (uint32_t)addr / PAGE_SIZE;
	if (page_number >= b.size) return (uint32_t)-1;
	bitmap_clear(&b, page_number);

	return 0;

}






