#include "strcompare.h"
#include "strlength.h"
#include <stdint.h>

uint32_t strcompare(const char *first, const char *second){
	
	for (uint32_t i = 0; i < strlength(second); i++) {
      		if (first[i] != second[i]) return 0;
	}

	return 1;
}
