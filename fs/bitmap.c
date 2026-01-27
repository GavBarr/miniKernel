#include <stdint.h>
#include <stddef.h>
#include "bitmap.h"
#include "../include/memset.h"
#include "../mem_alloc/heap.h"


int fs_bitmap_init(struct Bitmap *b, uint32_t num_bits){
        if (!b || num_bits == 0) return -1;

        uint32_t bytes = (num_bits + 7) / 8;
        void *ptr = kmalloc(bytes);
        if (ptr == NULL) return -1;

	int status = kmemset(ptr, 0, bytes);//kmalloc()//memset((void *)ptr, 0, bytes);
	
        if (status != 0){
	       	kfree(ptr);
		return -1;
	}


        b->data = ptr;
        b->size = bytes;

        return 0;

}

int fs_bitmap_set(struct Bitmap *b, uint32_t bit_index){
        uint32_t byte_index = bit_index / 8;
        uint8_t bit_offset = bit_index % 8;

        b->data[byte_index] = b->data[byte_index] | (1 << bit_offset);

        return 0;


}


int fs_bitmap_clear(struct Bitmap *b, uint32_t bit_index){

        uint32_t byte_index = bit_index / 8;
        uint8_t bit_offset = bit_index % 8;

        b->data[byte_index] = b->data[byte_index] & ~(1 << bit_offset);

        return 0;
}


int fs_bitmap_test(struct Bitmap *b, uint32_t bit_index){
//check if a bit is used or free
        uint32_t byte_index = bit_index / 8;
        uint8_t bit_offset = bit_index % 8;

        uint8_t byte = b->data[byte_index];

        if(byte &(1 << bit_offset)){
                return 1;
        }

        return 0;


}
uint32_t fs_bitmap_find_free(struct Bitmap *b){

        uint32_t bytes = b->size;// / 8;

        for (uint32_t i = 0; i < bytes; i++){
                uint8_t byte = b->data[i];

                if (byte == 0xFF) continue;
                for(uint8_t bit_offset = 0; bit_offset < 8; bit_offset++){
                        if(!(byte & (1 << bit_offset))){
                                return i * 8 + bit_offset; // 2 * 8 = 16 + 2 bits = 18 bit is the first free bit
                        }

                }



        }

        return -1;

}

int fs_bitmap_write(struct Bitmap *b, uint32_t block_number, uint32_t block_size ,struct block_device *disk){
	if (disk == NULL) return -1;

        int records_written = disk->ops->write_block(disk, block_number, (void *)b);
        if (records_written != 0) return -1;


        return 0;

}


int fs_bitmap_read(struct Bitmap *b, uint32_t block_number, uint32_t block_size,struct block_device *disk){
        if (disk == NULL) return -1;
	
        int records_read = disk->ops->read_block(disk, block_number, b);//fread(b->data, 1, b->size, disk);

	if (records_read != 0) return -1;

        return 0;


}
