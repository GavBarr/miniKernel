#include "shell.h"
#include "../drivers/keyboard.h"
#include "../drivers/screen.h"
#include "../debug/debug.h"
#include "../mem_alloc/heap.h"
#include "../include/strcompare.h"
#include <stddef.h>

void shell_run(){

	print_string("goose>");
	while (1){
		char character = keyboard_getchar();
		//if (character == '\n'){
		//	char *command = parse_command(get_keyboard_buffer());
		//	if (strcompare(command,"echo\0")){
		//		uint32_t len = 5;
		//		char *arg = parse_arg(get_keyboard_buffer(), len);
				//kfree((void *)command);

		//		if (arg != NULL){
					//kfree(arg);					
		//		}
		//	}
		//}
        	if (character != 0){
			display_character(character);
		}
	}
}


