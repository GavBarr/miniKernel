#ifndef IDE_ATA_DRIVER_H
#define IDE_ATA_DRIVER_H
#include <stdint.h>
#include "../include/block_device.h"

struct ide_device{
	uint8_t channel;
	uint8_t type;
	uint8_t drive;
	uint16_t signature;
	uint16_t capabilities;
	uint32_t command_sets;
	uint8_t irq;
	char model[41];
	uint8_t exists;
	uint64_t size;
};

struct ide_channel{
	uint16_t base_io_port;
	uint16_t control_port;
	uint8_t irq;
	uint8_t int_enable_state;

};

void ide_init();
void ide_read_block();
void ide_primary_irq_handler();
void ide_secondary_irq_handler();

#endif

