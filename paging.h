#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

//Page Directory -> Page Directory Entry -> Page Table -> Page Table Entry


//PDE
//4 bytes
struct page_directory_entry{

	uint32_t present    : 1; //is this entry valid
	uint32_t rw         : 1; //0=read only 1=read-write
	uint32_t user       : 1; //0=supervisor, 1=user accessible
	uint32_t pwt        : 1; //page write-through
	uint32_t pcd        : 1; //page cache disabled
	uint32_t accessed   : 1; //has been accessed?
	uint32_t available1 : 1; //available for OS use
	uint32_t page_size  : 1; //0=4kb, 1=4mb pages
	uint32_t available2 : 4; //available for OS use
	uint32_t frame      : 20;//physical address of page table >> 12
	

	

}__attribute__((packed));

	
//PTE - kind of like chapteres within the table of contents aka the PDE
//4 bytes
struct page_table_entry{
	uint32_t present   : 1;
        uint32_t rw        : 1;
        uint32_t user      : 1;
        uint32_t pwt       : 1;
        uint32_t pcd       : 1;
        uint32_t accessed  : 1;
        uint32_t dirty     : 1; //has been written to?
        uint32_t pat       : 1; //page attribute table
        uint32_t global    : 1; //global page
        uint32_t available : 3; //available for OS use
        uint32_t frame     : 20;//physical address of page >> 12


}__attribute__((packed));

//PD
//4 bytes
struct page_directory{
        struct page_directory_entry entries[1024];


}__attribute__((packed));


//PT
struct page_table{
        struct page_table_entry entries[1024];

}__attribute__((packed));


void page_dir_init();
void enable_paging();
void load_page_directory(uint32_t page_dir);


#endif
