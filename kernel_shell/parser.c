#include "../debug/debug.h"
#include "../mem_alloc/heap.h"
#include "../include/strlength.h"
#include "parser.h"
#include <stdint.h>
#include <stddef.h>

char *parse_command(char buffer[]){
	uint32_t index = 0;
	uint32_t len = 0;
	while (buffer[index] != ' ' && buffer[index] != '\n'){
		len++;
		index++;
	}

	index=0;
	char *command = (char *)kmalloc(len + 1);

	for (uint32_t i = 0; i < len; i++){
		command[i] = buffer[i];
	}
	command[len] ='\0';

	return command;
}

char *parse_arg(char buffer[], uint32_t start_index){
	uint32_t i = 0;
	uint32_t j = 0;
	uint32_t len = strlength(buffer) - start_index;
	char *arg = (char *)kmalloc(len);

	for (uint32_t i = start_index; i < start_index + len; i++){
		
		arg[j] = buffer[i];
		j++;
	}
	arg[len] = '\0';

	return arg;

}

