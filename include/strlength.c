#include "strlength.h"
#include <stdint.h>

uint32_t strlength(const char *string){	
	uint32_t i = 0;
	uint32_t len = 0;
	while (string[i] != '\0' && string[i] != '\n'){
		i++;
		len++;
	}
	return len;
}
