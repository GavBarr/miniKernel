#ifndef GDT_H
#define GDT_H
#include <stdint.h>

//MUST be 8bytes, the order of the bytes matter significantly and must match this
struct gdt_entry{
	
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t granularity;
	uint8_t base_high;


};

struct gdt_ptr{

	uint16_t limit;
	uint32_t base;

};


void gdt_init(void);

#endif

