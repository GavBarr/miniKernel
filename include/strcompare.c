#include "strcompare.h"
#include <stdint.h>

uint32_t strcompare(const char *first, const char *second){
	
	while (*first && *second){
		if (*first != *second) return 0;
		first++;
		second++;
	}
	return *first == *second;
}
