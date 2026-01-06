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

}__attribute__((packed));


void idt_init(void);
void idt_set_gate(uint8_t index, uint32_t handler_address, uint16_t selector, uint8_t flags);
void isr_handler(uint32_t interrupt_number);
extern void idt_load(uint32_t);
static inline void outb(uint16_t port, uint8_t value);
static inline uint8_t inb(uint16_t port);
void pic_remap(uint8_t offset1, uint8_t offset2);


#endif

