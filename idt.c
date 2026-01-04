#include "idt.h"

struct idt_ptr i_ptr;
struct idt_entry idt[256];


void idt_init(void){
	//gdt_add_entry(1, 0, 0xFFFFF, 0x9A, 0xCF);
	//gdt_add_entry(2, 0, 0xFFFFF, 0x92, 0xCF);

	i_ptr.limit = sizeof(idt) -1;
	i_ptr.base = (uint32_t)&idt;

	idt_load((uint32_t)&i_ptr);

}

void gdt_add_entry(int entry_index, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags){

	//base_low is 16bits
	//high is 16 bits
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
