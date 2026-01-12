#include "bitmap.h"
#include "debug.h"
#include <stdint.h>

//struct bitmap{
//        uint8_t *data;
//        uint32_t size;
//
//};


int bitmap_init(struct bitmap *b, uint8_t *bitmap_data, uint32_t size){
	if (!b || size == 0) return -1;

        b->data = bitmap_data;
        b->size = size;

//	print_int(size);
	uint32_t num_bytes = (size + 7) / 8;
	for (uint32_t i = 0; i < num_bytes; i++){
		b->data[i] = 0xFF;
		//print_int(i);
	}

        return 0;


}

int bitmap_set(struct bitmap *b, uint32_t bit_index){

	uint32_t byte_index = bit_index / 8;
        uint8_t bit_offset = bit_index % 8;

        b->data[byte_index] = b->data[byte_index] | (1 << bit_offset);

	return 0;

}

int bitmap_clear(struct bitmap *b, uint32_t bit_index){

	uint32_t byte_index = bit_index / 8;
        uint8_t bit_offset = bit_index % 8;

	b->data[byte_index] = b->data[byte_index] & ~(1 << bit_offset);


	return 0;
}

int bitmap_test(struct bitmap *b, uint32_t bit_index){

	uint32_t byte_index = bit_index / 8;
        uint8_t bit_offset = bit_index % 8;
	
	uint8_t byte = b->data[byte_index];

	if (byte &(1 << bit_offset)){
		return 1;
	}

	return 0;

	

}

uint32_t bitmap_find_free(struct bitmap *b){

		
	uint32_t size = b->size / 8;

	for (uint32_t i = 0; i < size; i++){
		uint8_t byte = b->data[i];
		if (byte == 0xFF) continue;

		//now loop through each bit, instead of byte
		for (uint8_t offset = 0; offset < 8; offset ++){
			if (!(byte & (1 << offset))){
				//print_int((i * 8) + offset);
				return (i * 8) + offset; 
			}
		}
	
	}
	
		
	return (uint32_t)-1;

}


