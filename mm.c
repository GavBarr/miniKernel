#include "mm.h"
#include "multiboot.h"
#include <stdint.h>
#include <stddef.h>

void mmap_init(void){


}

uint32_t get_total_entries(struct mmap mem_map){
	uint8_t *ptr = (uint8_t *)mem_map.addr;
        uint32_t total_length = mem_map.len;
        uint32_t offset = 0;
        uint32_t arr_size = 0;
        while (offset < total_length){

                struct mmap_entry *entry = (struct mmap_entry *)ptr;

                uint32_t advance = sizeof(uint32_t) + entry->size;
                offset += advance;
                ptr += advance;
                arr_size++;
        }

	return arr_size;

}


void create_mmap_entries(struct mmap mem_map, struct mmap_entry mm_entries[]){
	uint8_t *ptr = (uint8_t *)mem_map.addr;
        uint32_t total_length = mem_map.len;
        uint32_t offset = 0;
        uint32_t arr_size = 0;
        uint32_t index = 0;

        while(offset < total_length){

                struct mmap_entry *entry = (struct mmap_entry *)ptr;

                uint32_t size = entry->size;
                uint64_t addr = entry->addr;
                uint64_t len = entry->len;
                uint32_t type = entry->type;

                create_mmap_entry(mm_entries, index, size, addr, len, type);

                uint32_t advance = sizeof(uint32_t) + entry->size;

                offset += advance;
                ptr += advance;

                index++;

        }

}

void copy_mmap(multiboot_info_t *mbi, struct mmap mem_map){

	mem_map.addr=mbi->mmap_addr;
	mem_map.len=mbi->mmap_length;
	
}

void create_mmap_entry(struct mmap_entry entries[],uint32_t index, uint32_t size, uint64_t addr, uint64_t len, uint32_t type){
		
	if (entries[index].size == 0){
		entries[index].size = size;
		entries[index].addr = addr;
		entries[index].len = len;
		entries[index].type = type;
	}
}



uint64_t find_highest_address(struct mmap_entry entries[], uint32_t entry_count){
	uint32_t mmap_entry_size = entry_count / sizeof(entries[0]);
	uint64_t highest_addr;

	for (uint32_t i = 0; i < mmap_entry_size; i++){

		if (entries[i].type == 1){ //indicates that its usable
			
			if(highest_addr < (entries[i].addr + entries[i].len)){
				highest_addr = entries[i].addr + entries[i].len;
			}

		}	
	
	}

	return highest_addr;


}





