#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>

struct bitmap{
        uint8_t *data;
        uint32_t size;

};


int bitmap_init(struct bitmap *b, uint8_t *bitmap_data,uint32_t size);
int bitmap_set(struct bitmap *b, uint32_t bit_index);
int bitmap_clear(struct bitmap *b, uint32_t bit_index);
int bitmap_test(struct bitmap *b, uint32_t bit_index);
uint32_t bitmap_find_free(struct bitmap *b);


#endif
