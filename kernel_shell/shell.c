#include "shell.h"
#include "../drivers/keyboard.h"
#include "../drivers/screen.h"
#include "../debug/debug.h"
#include "../mem_alloc/heap.h"
#include "../include/strcompare.h"
#include <stddef.h>

void shell_run(){
	print_string("._____._____._____._____._____.\n\0");
	print_string("|  _  |  _  |  _  |__ --|  -__|\n\0");
	print_string("|___  |_____|_____|_____|_____|\n\0");
	print_string("|_____|\n\0");
	print_string("goose>\n\0");
	while (1){
		char character = keyboard_getchar();
        	if (character != 0){
			display_character(character);
		}
	}
}


