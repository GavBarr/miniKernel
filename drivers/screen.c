#include "../debug/debug.h"
#include "../include/strlength.h"
#include "../include/strcompare.h"
#include "../kernel_shell/parser.h"
#include "keyboard.h"
#include "screen.h"
#include <stdint.h>
#include <stddef.h>
#define PROMPT_COLOR 0x0B

static uint32_t cursor_pos = 6;
static uint32_t row = 1;
volatile char *video_cursor = (volatile char *)0xB8000;
static void print_prompt();
static void print_command_err();
static void print_echo_arg(char *arg);
static inline void outb(uint16_t port, uint8_t val);

void display_character(char character){
	if (character == '\b'){
		if (cursor_pos > ((row - 1) * 160) + 6){
			cursor_pos--;
			video_cursor[cursor_pos * 2] = ' ';
			video_cursor[cursor_pos * 2 + 1] = 0x0F;

		}
		return;
	}

	if (character == '\n'){
                char *command = parse_command(get_keyboard_buffer());
                if (strcompare(command,"echo\0")){

                	uint32_t len = 5;
                        char *arg = parse_arg(get_keyboard_buffer(), len);
                      	

                        if (arg[0] != '\0'){
				print_echo_arg(arg);
                              //kfree(arg);                                   
                        }else{

				print_command_err();
			}
		}else if(strcompare(command, "clear\n")){

			clear_screen();

		}
		


		clear_keyboard_buffer();
		print_prompt();
		return;	
	}

	
	video_cursor[cursor_pos * 2] = character;
        video_cursor[cursor_pos * 2 + 1] = 0x0F;

	cursor_pos++;
        if (cursor_pos >= 80 * 25){
        	cursor_pos = 0;
	}

}
void clear_screen(void){
	uint32_t total_spaces = 80 * 25;
	for (uint32_t i = 0; i < total_spaces; i++){
		video_cursor[i * 2] = ' ';
	        video_cursor[i * 2 + 1] = 0x0F;
	}
	cursor_pos=0;

}

static void print_command_err(){
	cursor_pos += (80 - (6 + strlength("echo")));
	char *text = "echo: missing arg";
        uint32_t len = strlength("echo: missing arg");
        for (uint32_t i = 0; i < len; i++){
                video_cursor[cursor_pos * 2] = text[i];
                video_cursor[cursor_pos * 2 + 1] = 0x0F;
                cursor_pos++;

        }

}

static void print_echo_arg(char *arg){
	cursor_pos += (80 - (6 + strlength("echo") +strlength(arg)));

	uint32_t len = strlength(arg);
	for (uint32_t i = 0; i < len; i++){
		if (arg[i] == '\n' || arg[i] == '\0') continue;
		video_cursor[cursor_pos * 2] = arg[i];
        	video_cursor[cursor_pos * 2 + 1] = 0x0F;
	        cursor_pos++;

	}

}

static void print_prompt(){
	
	cursor_pos = row * 160;

	if (row != 12){
                row++;
        }else{
                cursor_pos = 0;
                row = 1;
		clear_screen();
        }


	video_cursor[cursor_pos * 2] = 'g';
        video_cursor[cursor_pos * 2 + 1] = PROMPT_COLOR;
        cursor_pos++;
	video_cursor[cursor_pos * 2] = 'o';
        video_cursor[cursor_pos * 2 + 1] = PROMPT_COLOR;
        cursor_pos++;
	video_cursor[cursor_pos * 2] = 'o';
        video_cursor[cursor_pos * 2 + 1] = PROMPT_COLOR;
        cursor_pos++;
	video_cursor[cursor_pos * 2] = 's';
        video_cursor[cursor_pos * 2 + 1] = PROMPT_COLOR;
        cursor_pos++;
	video_cursor[cursor_pos * 2] = 'e';
        video_cursor[cursor_pos * 2 + 1] = PROMPT_COLOR;
        cursor_pos++;
	video_cursor[cursor_pos * 2] = '>';
        video_cursor[cursor_pos * 2 + 1] = PROMPT_COLOR;
        cursor_pos++;


	
}

