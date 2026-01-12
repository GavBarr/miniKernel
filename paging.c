#include "paging.h"
#include "debug.h"
#include "mem_alloc.h"
#include <stdint.h>


#define NUM_ENTRIES 1024

static struct page_directory *pd;
static struct page_table *kernel_pt;

void page_dir_init(){

	void *ptr = kmalloc_page();
	pd = (struct page_directory *)ptr;
	//print_pointer(ptr);

	for (uint32_t entry = 0; entry < NUM_ENTRIES; entry++){
		pd->entries[entry].present = 0;
		pd->entries[entry].rw = 0;
		pd->entries[entry].user = 0;
		pd->entries[entry].pwt = 0;
		pd->entries[entry].pcd = 0;
		pd->entries[entry].accessed = 0;
		pd->entries[entry].available1 = 0;
		pd->entries[entry].page_size = 0;
		pd->entries[entry].available2 = 0;
		pd->entries[entry].frame = 0;
	}

	//Kernel Page Table
	void *kernel_ptr = kmalloc_page();
	kernel_pt = (struct page_table *)kernel_ptr;
	//print_pointer(kernel_ptr);

	pd->entries[0].present = 1;
	pd->entries[0].rw = 1;
	pd->entries[0].user = 0;
	pd->entries[0].frame = ((uint32_t)kernel_ptr) >> 12;
	for (uint32_t entry = 0; entry < NUM_ENTRIES; entry++){
		uint32_t physical_addr = entry * 0x1000;

		//void *next_addr = kmalloc_page();
		//if (entry == (uint32_t)1023) print_pointer(next_addr);
		kernel_pt->entries[entry].present = 1;
		kernel_pt->entries[entry].rw = 1;
		kernel_pt->entries[entry].user = 0;
		kernel_pt->entries[entry].pwt = 0;
		kernel_pt->entries[entry].pcd = 0;
		kernel_pt->entries[entry].accessed = 0;
		kernel_pt->entries[entry].dirty = 0;
		kernel_pt->entries[entry].pat = 0;
		kernel_pt->entries[entry].global = 0;
		kernel_pt->entries[entry].available = 0;
		kernel_pt->entries[entry].frame = physical_addr >> 12;
	}
}










