#include "../debug/debug.h"
#include "heap.h"
#include "mem_alloc.h"
#include <stdint.h>
#include <stddef.h>
#define HEAP_SIZE 65536 //64kb
#define PAGE_SIZE 4 //4kb
#define MINIMUM_BLOCK_SIZE 16 //16kb		
//struct block_header{
//        uint32_t size;
//        uint32_t free_flag;
//        uint32_t magic;
//};
static void *find_free_block(uint32_t size);
static void split_block(struct block_header *block, uint32_t size);

static void *heap_start;
static void *heap_end;
static struct block_header *first_block;
static uint32_t debug = 0; //internal use for debugging

void heap_init(){
	heap_start = kmalloc_page();
	if (heap_start == NULL) return;
	for (uint32_t i = 0; i < 15; i++){
		void *page = kmalloc_page();
		if (page == NULL) return;
	}
	first_block = (struct block_header *)heap_start;
	first_block->size = HEAP_SIZE - sizeof(struct block_header);
	first_block->free_flag = 1;
	first_block->magic=0xDEADBEEF;
	heap_end = (void *)((uint32_t)heap_start + HEAP_SIZE); 
	if (debug == 1){
		print_string("heap start -> \0");
		print_pointer(heap_start);
		print_string("\n\0");
		print_string("first_block.size -> \0");
		print_int((uint32_t)first_block->size);
		print_string("\n\0");
                print_string("first_block.free_flag -> \0");
                print_int((uint32_t)first_block->free_flag);
		print_string("\n\0");
                print_string("first_block.magic -> \0");
                print_pointer((void *)first_block->magic);
	}
}
void *kmalloc(uint32_t size){
	struct block_header *block = find_free_block(size);
	if (block == NULL) return NULL;
	split_block(block, size);

	block->free_flag = 0; //used block now
	return block;
}
void kfree(void *addr);
static void *find_free_block(uint32_t size){
	if (size > HEAP_SIZE) return NULL;
	struct block_header *current_block = first_block;

	while ((void *)current_block<heap_end){
		if (current_block->magic != 0xDEADBEEF) return NULL;
		if (current_block->size >= size && current_block->free_flag == 1){
			return current_block;
		}
		void *next_addr = (void *)((uint32_t)current_block+sizeof(struct block_header)+(current_block->size));
                current_block = (struct block_header *)next_addr;
	}
	return NULL;
}

//minimum leftover requirement = 16bytes
static void split_block(struct block_header *block, uint32_t size){
	uint32_t leftover_size = block->size - size;
	if (leftover_size > sizeof(struct block_header) + MINIMUM_BLOCK_SIZE){
		block->size = size;

		
		struct block_header *new_block = (struct block_header *)((uint32_t)block + sizeof(struct block_header) + size);
		new_block->size = leftover_size - sizeof(struct block_header);
		new_block->free_flag = 1;
		new_block->magic = 0xDEADBEEF;
		if (debug == 1){
			print_string("\n\0");
		        print_string("new_block_addr.addr -> \0");
		        print_pointer(new_block);
			print_string("\n\0");
	                print_string("new_block_addr.size -> \0");
	                print_int((uint32_t)new_block->size);
			print_string("\n\0");
	                print_string("new_block_addr.free_flag -> \0");
	                print_int((uint32_t)new_block->free_flag);
			print_string("\n\0");
	                print_string("new_block_addr.magic -> \0");
	                print_pointer((void *)new_block->magic);
		}
	}
}


static struct block_header get_block_header(void *ptr);








