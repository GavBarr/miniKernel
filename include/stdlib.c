#include "stdlib.h"
#include <stdint.h>
#include <stddef.h>

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

int kmemset(void *ptr, uint32_t val, uint32_t size){
        uint8_t *p = (uint8_t *)ptr;
        for (int i = 0; i < size; i++){
                p[i] = val;
        }

        return 0;
}

uint32_t strcompare(const char *first, const char *second){

        for (uint32_t i = 0; i < strlength(second); i++) {
                if (first[i] != second[i]) return 0;
        }

        return 1;
}

uint32_t strlength(const char *string){
        uint32_t i = 0;
        uint32_t len = 0;
        while (string[i] != '\0' && string[i] != '\n'){
                i++;
                len++;
        }
        return len;
}
