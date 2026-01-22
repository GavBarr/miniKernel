#include "ide_ata_driver.h"
#include "../mem_alloc/heap.h"
#include <stddef.h>
#include <stdint.h>

static inline void outb(uint16_t port, uint8_t value);
static inline uint8_t inb(uint16_t port);
static inline uint8_t inw(uint16_t port);
static void probe_channels();
//max possible drives
static struct ide_device *ide_dev[4];
//primary and secondory
static struct ide_channel *ide_chan[2];

void ide_init(){
	ide_dev = kmalloc(sizeof(struct ide_device) * 4);
	ide_chan = kmalloc(sizeof(struct ide_channel) * 2);
	if (ide_dev == NULL || ide_chan == NULL) return;

	//primary
	ide_chan[0]->base_io_port = 0x1F0;
	ide_chan[0]->control_port = 0x3F6;
	ide_chan[0]->irq = 14;
	ide_chan[0]->int_enable_state = 0;
	
	//secondary
	ide_chan[1]->base_io_port = 0x170;
        ide_chan[1]->control_port = 0x376;
        ide_chan[1]->irq = 15;
        ide_chan[1]->int_enable_state = 0;
	

	disable_interrupts();
	probe_channels();
	enable_interrupts();




}

void ide_read_block(uint8_t device, uint32_t lba, uint8_t *buffer){
	uint8_t channel = ide_dev[device]->channel;
	uint16_t base = ide_chan[channel]->base_io_port;
	uint8_t driver = ide_dev[device]->drive;

	//wait until driver is not busy
	while (inb(base + 7) & 0x80);

	uint8_t drive_type;

	//select driver using LBA mode, Logical Block Addressing
	//master
	if (channel == 0){
		drive_type = 0xE0;
	}else{
	//slave
		drive_type = 0xB0;
	}	
	
	//setting the upp4 bits, 24-27
	uint8_t lba_upper = (lba >> 24) & 0X0F;
	drive_type = drive_type | lba_upper;

	//actually selecting the drive requested, but with the upper bits set correctly 
	outb(base + 6, drive_type);
	while (inb(base + 7) & 0x80);

	outb(base + 2, 1); //read 1 512 bytes sector
	
	//set lba lower bits
	uint8_t lba_low = lba & 0xFF;
	outb(base + 3, lba_low);

	uint8_t lba_mid = (lba >> 8) & 0xFF;
	outb(base + 4, lba_mid);

	uint8_t lba_high = (lba >> 16) & 0xFF;
	outb(base + 5, lba_high);


	//READ SECTORS command
	outb(base + 7, 0x20);

	while (inb(base + 7) & 0x80);

	uint8_t status = inb(base + 7);

	//error
	if (stats == 0x01){
		return -1;
	}
	//device fault
	if (status & 0x20){
		return -1;
	}
	//wait until DRQ is ready
	while(!(inb(base + 7) & 0x08));

	uint16_t *buf = (uint16_t *)buffer;

	for(int i = 0; i < 256; i++){
		buf[i] = inw(base + 0);
	}
	
	return 0;

}

void ide_primary_irq_handler(){
	uint8_t status = inb(ide_chan[0]->base_io_port + 7);

	//if there is any error then address it accordingly
	if (status & 0x01){
	
	}

	//if data is ready (DRQ bit)
	if (status & 0x08){
	
	}
}

void ide_secondary_irq_handler(){
	uint8_t status = inb(ide_chan[1]->base_io_port + 7);

	//if there is any error then address it accordingly
        if (status & 0x01){

        }

        //if data is ready (DRQ bit)
        if (status & 0x08){

        }

}

static void disable_interrupts(){
	//we must disable interrupts while probing
        for (int i = 0; i < 2; i++){
                uint16_t control = ide_chan[i]->control_port;
                uint16_t status = ide_chan[i]->base_io_port + 7;

                uint8_t val = inb(control);
                outb(control, 0x02); //use the or bitwise operation, to ONLY disable IRQs while maintaining other bits
                io_wait();

        }

}

static void enable_interrupts(){
        //we must disable interrupts while probing
        for (int i = 0; i < 2; i++){
                uint16_t control = ide_chan[i]->control_port;
                uint16_t status = ide_chan[i]->base_io_port + 7;

                uint8_t val = inb(control);
                outb(control, 0x00); //use the or bitwise operation, to ONLY disable IRQs while maintaining other bits
                io_wait();

        }

}


//essentially looking to see drives are out there and what's connected
static void probe_channels(){
	//go through both primary and secondary
	for (int channel = 0; channel < 2; channel++){
		uint16_t base = ide_chan[channel]->base_io_port;
                uint16_t status = base + 7; //looking at the status register
		uint16_t command = base + 7;
		for(int device = 0; device < 2; device++){
			//this is to select which drive we want to be using
			if (device == 0){
				outb(base + 6, 0xE0); //Master device for the current channel
			}else{
				outb(base + 6, 0xB0); //Slave device for the current channel
			}
			io_wait();//we want a small delat after selecting the drive
			while(inb(status) & 0x80);//we only want to look at bit 7, so we need to single it out using the AND operation
		
			//we MUST zero out the sector count, lba low, lba mid and lba high. According to ATA standards	
			outb(base + 2, 0);
			outb(base + 3, 0);
			outb(base + 4, 0);
			outb(base + 5, 0);
			
			outb(command, 0xEC);//send out IDENTIFY command
			while(inb(status) & 0x80);
			uint8_t s = inb(base + 7);
			if (s == 0x00 || s == 0xFF) continue;
			if (s & 0x01) continue; //if ERR is set, then it failed
			while(!(inb(base + 7) & 0x08)); //if the DRQ is set to 0, meaning that the data isn't ready, just wait and loop
			uint16_t identify_data[256];
			for (int i = 0; i < 256; i++){
				identify_data[i] = inw(base + 0); //trying to be explicit with base
			
			}
			//after putting the devcice info data into identify_data[] I need to parse
			//out the specific data that is needed for the ide_device struct etc
			//word 0: general info, bit 15 tells if ATA or ATAPI
			//words 10-19: serial number
			//words 27-46: model number
			//word 49: capabilites (lba support etc.)
			//words 60-61: total sectors addressable in LBA28 mode (32-bit value split across the two words)
			//word 83: command sets supported (tells if LBA48 is supported)
			//words 100-103: total secotrs addressable in LBA48 mode (48-bit value across four words)
		
			device_placeholder = device;
			if (channel > 0){
				device_placeholder += 2;
			}
			
			ide_dev[device_placeholder]->channel = channel;	
			ide_dev[device_placeholder]->type = 0;//hard setting this becuase I'm only using hard drives
			ide_dev[device_placeholder]->drive = (device_placeholder);	
			ide_dev[device_placeholder]->signature = identify_data[0];	
			ide_dev[device_placeholder]->capabilities = identify_data[49];	
			ide_dev[device_placeholder]->command_sets = identify_data[83];	
			ide_dev[device_placeholder]->irq = ide_chan[channel].irq;
			//each word will contain 2 characters, but they are swapped, which is why we must swap in the loop
			for (int k = 0; k < 20; k++){
				ide_dev[device_placeholder]->model[k * 2] = (identify_data[27 + k] >> 8) & 0xFF;//high byte
				ide_dev[device_placeholder]->model[k * 2 + 1] = identify_data[27 + k] & 0xFF;//low byte
	
			}
			ide_dev[device_placeholder]->model[40] = '\0'; //null terminator
			ide_dev[device_placeholder]->exists = 1;
	
			if (identify_data[83] & (1 << 10)){//use words 100-103 for a 48-bit sector count
				ide_dev[device_placeholder]->size = ((uint64_t)identify_data[103] << 48) |
					           ((uint64_t)identify_data[102] << 32) |
						   ((uint64_t)identify_data[101] << 16) |
						   ((uint64_t)identify_data[100]);
			}else{//use words 60-61 for 32 bit sector count
			      	ide_dev[device_placeholder]->size = (identify_data[61] << 16) | identify_data[60];//upper 16bits + lower 16bits
			}
		}
		
	}

}

static inline void io_wait(){
	//this is basically a port was used in BIOS, so we use this to burn through a few cycles reliably
	outb(0x80, 0);
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

static inline uint8_t inw(uint16_t port){
        uint8_t return_val;

        __asm__ volatile ("inw %1, %0"
                         : "=a"(return_val)
                         : "Nd"(port)
                         );

        return return_val;
}

