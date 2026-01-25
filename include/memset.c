#include "memset.h"
#include <stdint.h>
#include <stddef.h>

int kmemset(void *ptr, uint32_t val, uint32_t size){
	uint8_t *p = (uint8_t *)ptr;
	for (int i = 0; i < size; i++){
		p[i] = val;	
	}
	
	return 0;
}


