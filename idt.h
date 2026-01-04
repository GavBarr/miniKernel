#ifndef IDT_H
#define IDT_H
#include <stdint.h>

struct idt_entry{
	
	uint16_t handler_function;


};

struct gdt_ptr{

	uint16_t limit;
	uint32_t base;

};


void gdt_init(void);
void gdt_add_entry(int entry_index, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);
extern void gdt_flush(uint32_t gdt_ptr_address);

#endif

