#include "gdt.h"

struct gdt_ptr g_ptr;
struct gdt_entry gdt[3];
//0 - Null Descriptor
//1 - Kernel code segment descriptor
//2 - Kernel data segment descriptor


void gdt_init(void){
	//dont need to do this for [0] because it should all be NULL or rather 0
	gdt_add_entry(1, 0, 0xFFFFF, 0x9A, 0xCF);
	gdt_add_entry(2, 0, 0xFFFFF, 0x92, 0xCF);

	g_ptr.limit = sizeof(gdt) -1;
	g_ptr.base = (uint32_t)&gdt;

	gdt_flush((uint32_t)&g_ptr);

}

void gdt_add_entry(int entry_index, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags){

	//base_low is 16bits
	//middle is 8 bits
	//high is 8 bits
	//Bit position: 7  6  5  4  3  2  1  0
        //  	       [  ][  ][  ][  ][  ][  ][  ][  ]
        //    	       ↑                             ↑
        //             "high"                        "low"
        //             (most significant)        (least significant)
	

	gdt[entry_index].base_low = base & 0xFFFF;
	gdt[entry_index].base_middle = (base >> 16) & 0xFF;
	gdt[entry_index].base_high = (base >> 24) & 0xFF;

	gdt[entry_index].limit_low = limit & 0xFFFF;
	uint8_t limit_high = (limit >> 16) & 0x0F;

	gdt[entry_index].granularity = ( (limit_high & 0x0F) | (flags & 0xF0));
	gdt[entry_index].access = access;


}
