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

int memmove(void *destination, void *source, uint32_t length){
	if (destination == source || length == 0) return -1;

	char *dst = (char *)destination;
        char *src = (char *)source;
	
	
	if (dst < src){
	
        	for (uint32_t i = 0; i < length; i++){
                	dst[i] = src[i];
	        }
	}else{
		for (uint32_t i = length -1; i != 0; i--){
			dst[i - 1] = src[i - 1];
		}
	}

        return 0;


}
