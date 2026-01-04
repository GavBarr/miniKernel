#ifndef IDT_H
#define IDT_H
#include <stdint.h>

struct idt_entry{
	
	uint16_t base_low;
	uint16_t selector;
	uint8_t zero; //always zero, unused by CPU
	uint8_t flags;
	uint16_t base_high;
	

};

struct idt_ptr{

	uint16_t limit;
	uint32_t base;

};


void idt_init(void);
extern void idt_load(uint32_t base)

#endif

