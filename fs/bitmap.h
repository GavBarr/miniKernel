#ifndef BITMAP_H
#define BITMAP_H
#include <stdint.h>
#include "../include/block_device.h"

struct Bitmap{
        uint8_t *data;
        uint32_t size;

};

int fs_bitmap_init(struct Bitmap *b, uint32_t num_bits);
int fs_bitmap_set(struct Bitmap *b, uint32_t bit_index);
int fs_bitmap_clear(struct Bitmap *b, uint32_t bit_index);
int fs_bitmap_test(struct Bitmap *b, uint32_t bit_index);
uint32_t fs_bitmap_find_free(struct Bitmap *b);
int fs_bitmap_write(struct Bitmap *b, uint32_t block_number, uint32_t block_size, struct block_device *disk);
int fs_bitmap_read(struct Bitmap *b, uint32_t block_number, uint32_t block_size, struct block_device *disk);
int fs_test_bitmap(void);

#endif
