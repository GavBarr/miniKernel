#include "memcopy.h"
#include <stdint.h>


int memcopy(void *destination, void *source, uint32_t length){
	
	char *dst = (char *)destination;
	char *src = (char *)source;

	for (int i = 0; i < length; i++){
		dst[i] = src[i];
	}
	return 0;
}
