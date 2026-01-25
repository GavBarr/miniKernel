#ifndef BITMAP_H
#define BITMAP_H
#include <stdint.h>
#include "../include/block_device.h"

struct Bitmap{
        uint8_t *data;
        uint32_t size;

};

int bitmap_init(struct Bitmap *b, uint32_t num_bits);
int bitmap_set(struct Bitmap *b, uint32_t bit_index);
int bitmap_clear(struct Bitmap *b, uint32_t bit_index);
int bitmap_test(struct Bitmap *b, uint32_t bit_index);
uint32_t bitmap_find_free(struct Bitmap *b);
int bitmap_write(struct Bitmap *b, uint32_t block_number, uint32_t block_size, struct block_device *disk);
int bitmap_read(struct Bitmap *b, uint32_t block_number, uint32_t block_size, struct block_device *disk);
int test_bitmap(void);

#endif
