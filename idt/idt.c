#include "idt.h"
#include "../keyboard.h"
#include "../debug/debug.h"

struct idt_ptr i_ptr;
struct idt_entry idt[256];

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);
extern void isr32(void);
extern void isr33(void);
extern void isr34(void);
extern void isr35(void);
extern void isr36(void);
extern void isr37(void);
extern void isr38(void);
extern void isr39(void);
extern void isr40(void);
extern void isr41(void);
extern void isr42(void);
extern void isr43(void);
extern void isr44(void);
extern void isr45(void);
extern void isr46(void);
extern void isr47(void);



void idt_init(void){



	idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
	idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
	idt_set_gate(2, (uint32_t)isr2, 0x08, 0x8E);
	idt_set_gate(3, (uint32_t)isr3, 0x08, 0x8E);
	idt_set_gate(4, (uint32_t)isr4, 0x08, 0x8E);
	idt_set_gate(5, (uint32_t)isr5, 0x08, 0x8E);
	idt_set_gate(6, (uint32_t)isr6, 0x08, 0x8E);
	idt_set_gate(7, (uint32_t)isr7, 0x08, 0x8E);
	idt_set_gate(8, (uint32_t)isr8, 0x08, 0x8E);
	idt_set_gate(9, (uint32_t)isr9, 0x08, 0x8E);
	idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
	idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
	idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
	idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
	idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
	idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
	idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
	idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
	idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
	idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
	idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
	idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
	idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
	idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
	idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
	idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
	idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
	idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
	idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
	idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
	idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
	idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);
	idt_set_gate(32, (uint32_t)isr32, 0x08, 0x8E);
	idt_set_gate(33, (uint32_t)isr33, 0x08, 0x8E);
	idt_set_gate(34, (uint32_t)isr34, 0x08, 0x8E);
	idt_set_gate(35, (uint32_t)isr35, 0x08, 0x8E);
	idt_set_gate(36, (uint32_t)isr36, 0x08, 0x8E);
	idt_set_gate(37, (uint32_t)isr37, 0x08, 0x8E);
	idt_set_gate(38, (uint32_t)isr38, 0x08, 0x8E);
	idt_set_gate(39, (uint32_t)isr39, 0x08, 0x8E);
	idt_set_gate(40, (uint32_t)isr40, 0x08, 0x8E);
	idt_set_gate(41, (uint32_t)isr41, 0x08, 0x8E);
	idt_set_gate(42, (uint32_t)isr42, 0x08, 0x8E);
	idt_set_gate(43, (uint32_t)isr43, 0x08, 0x8E);
	idt_set_gate(44, (uint32_t)isr44, 0x08, 0x8E);
	idt_set_gate(45, (uint32_t)isr45, 0x08, 0x8E);
	idt_set_gate(46, (uint32_t)isr46, 0x08, 0x8E);
	idt_set_gate(47, (uint32_t)isr47, 0x08, 0x8E);

	pic_remap(32, 40);


	i_ptr.limit = sizeof(idt) -1;
	i_ptr.base = (uint32_t)&idt;

	idt_load((uint32_t)&i_ptr);

	__asm__ volatile("sti");
}

void idt_set_gate(uint8_t index, uint32_t handler_address, uint16_t selector, uint8_t flags){

	//base_low is 16bits
	//high is 16 bits
	//Bit position: 7  6  5  4  3  2  1  0
        //  	       [  ][  ][  ][  ][  ][  ][  ][  ]
        //    	       ↑                             ↑
        //             "high"                        "low"
        //             (most significant)        (least significant)
	

	idt[index].base_low = handler_address & 0xFFFF;
	idt[index].base_high = (handler_address >> 16) & 0xFFFF;
	idt[index].selector = selector;
	idt[index].zero = 0;
	idt[index].flags = flags;


}

void isr_handler(uint32_t interrupt_number){
//	volatile char *v = (volatile char *)0xB8000;
//	v[2] = 'I';
//	v[1] = 0x0F;
	

	if (interrupt_number == 14){
		uint32_t fault_addr;
		__asm__ volatile("mov %%cr2, %0" : "=r"(fault_addr));
		print_string("---PAGE FAULT---\n\0");
		print_pointer((void *)fault_addr);

		while(1){

			__asm__ volatile("hlt");
		}
	}if (interrupt_number == 33) {
                keyboard_handler();
        }



	if (interrupt_number >= 32 && interrupt_number <= 47){
		if (interrupt_number >= 40){
			outb(0xA0, 0x20);//slave

		}	
		
		outb(0x20, 0x20);//send EOI for master and slave because of cascading to master
	}


}

static inline void outb(uint16_t port, uint8_t value){
	//porting I/O
	
	//assembly instructions ot put value into AL register and put port into DX register
	__asm__ volatile ("outb %0, %1"
			  :
			  : "a"(value),"Nd"(port)
			  );

}

static inline uint8_t inb(uint16_t port){
	uint8_t return_val;

	__asm__ volatile ("inb %1, %0"
			 : "=a"(return_val)
			 : "Nd"(port)
                         );

	return return_val;
}

//offset1 = master offset2 = slave
void pic_remap(uint8_t offset1, uint8_t offset2){
	
	//we are getting the current state before manipulation, because we do
	//not want to assume that all bits are 1's (or disabled), it could
	//have enabled interrupts currently happening and if we change that it
	//could negatively affect the CPU
	uint8_t master_mask = inb(0x21); //mask of the master bits
	uint8_t slave_mask = inb(0xA1); //mask of the slave bits

	//the PIC needs 4 initalization commands (ICW1-ICW4) to set up for 8086 mode and vector remapping
		
	//ICW1
	//master
	outb(0x20, 0x11); //0x11 means init in cascade mode 
	//slave
	outb(0xA0, 0x11);

	//ICW2
	outb(0x21, offset1);
	outb(0xA1, offset2);

	//ICW3
	outb(0x21, 0x04);
	outb(0xA1, 0x02);


	//ICW4
	outb(0x21, 0x01);
	outb(0xA1, 0x01);

	//basically restore the masks so that the bits are what they were before
	outb(0x21, master_mask);
	outb(0xA1, slave_mask);


}
