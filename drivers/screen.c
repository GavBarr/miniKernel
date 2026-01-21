#include "../debug/debug.h"
#include "../include/strlength.h"
#include "../include/strcompare.h"
#include "../kernel_shell/parser.h"
#include "../mem_alloc/heap.h"
#include "keyboard.h"
#include "screen.h"
#include <stdint.h>
#include <stddef.h>
#define PROMPT_COLOR 0x0B

static uint32_t row = 3;
static uint32_t cursor_pos = 326;
volatile char *video_cursor = (volatile char *)0xB8000;
static void print_prompt();
static void print_command_err();
static void print_echo_arg(char *arg);
static void print_heap();
static void print_total_heap();
static inline void outb(uint16_t port, uint8_t val);
static char *convert_int_to_char_arr(uint32_t n);
static char *convert_pointer_to_char_arr(void *ptr);
static uint32_t display_vga_text(char *buffer, uint32_t len, uint32_t temp_cursor_pos);
static void malloc_and_print(char *size);
static uint32_t convert_char_to_int(char *c);


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
                        }
			if (arg) kfree(arg);
			kfree(command);
		}else if(strcompare(command, "clear\n")){
			kfree(command);
			clear_screen();
			row=0;
		}else if(strcompare(command, "heap-start")){
			kfree(command);
			print_heap();
		}else if(strcompare(command, "heap-total")){
			kfree(command);
                        print_total_heap();
                }else if(strcompare(command, "kmalloc\0")){
			uint32_t len = 8;
                        char *arg = parse_arg(get_keyboard_buffer(), len);
                        if (arg[0] != '\0'){
                                malloc_and_print(arg);
                        }
                        if (arg) kfree(arg);
			kfree(command);

			
		}else{
			kfree(command);
			print_command_err();
		}
		
		clear_keyboard_buffer();
		print_prompt();
		//kfree(command);
		//walk_and_print_heap();
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
	uint32_t buffer_len = cursor_pos - 6;
	uint32_t temp_cursor_pos = ((row * 2) - 1) * 80;
	char *text = "err: invalid command";
        uint32_t len = strlength("err: invalid command");
        for (uint32_t i = 0; i < len; i++){
                video_cursor[temp_cursor_pos * 2] = text[i];
                video_cursor[temp_cursor_pos * 2 + 1] = 0xC;
                temp_cursor_pos++;

        }

}


static void print_heap(){
	
	uint32_t temp_cursor_pos = ((row * 2) - 1) * 80;	
	void *heap_start = get_heap_start();
	struct block_header *block = (struct block_header *)heap_start;
	
	char *addr = convert_pointer_to_char_arr(heap_start);
	char *size = convert_int_to_char_arr((uint32_t)block->size);
	char *free = convert_int_to_char_arr((uint32_t)block->free_flag);
	
	uint32_t size_len = strlength(size);
	uint32_t free_len = strlength(free);
	//addr	
	temp_cursor_pos = display_vga_text("address->", 9, temp_cursor_pos);
	temp_cursor_pos = display_vga_text(addr, 10, temp_cursor_pos);

	//size
	temp_cursor_pos = display_vga_text(" size->", 7, temp_cursor_pos);
	temp_cursor_pos = display_vga_text(size, size_len, temp_cursor_pos);

	//free_flag
	temp_cursor_pos = display_vga_text(" free_flag->", 12, temp_cursor_pos);
	temp_cursor_pos = display_vga_text(free, free_len, temp_cursor_pos);

	kfree(addr);
	kfree(size);
	kfree(free);
}


static void print_total_heap(){

        uint32_t temp_cursor_pos = ((row * 2) - 1) * 80;
        void *current = get_heap_start();
	void *end = get_heap_end();

	//hardcoded heap_end value for now TODO
	while (current < end){
		struct block_header *block = (struct block_header *)current;
	        char *addr = convert_pointer_to_char_arr(current);
		if (block->size != 0){
		       temp_cursor_pos = display_vga_text("address->", 9, temp_cursor_pos);
		       temp_cursor_pos = display_vga_text(addr, 10, temp_cursor_pos);
	               if (block->free_flag != 1) temp_cursor_pos = display_vga_text(" X ", 2, temp_cursor_pos);
	               temp_cursor_pos = display_vga_text("| ", 2, temp_cursor_pos);
		       kfree(addr);
		       row++;
		       temp_cursor_pos = ((row * 2) - 1) * 80;

		}else{
			break;
		}

		void *next = (void *)((uint32_t)current + sizeof(struct block_header) + (uint32_t)block->size);
		current = next;
	}
	

}


static void print_echo_arg(char *arg){
	cursor_pos += (80 - (6 + strlength("echo") +strlength(arg)));

	uint32_t len = strlength(arg);
	for (uint32_t i = 0; i < len; i++){
		if (arg[i] == '\n' || arg[i] == '\0' || arg[i] == '\b') continue;
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


static char *convert_int_to_char_arr(uint32_t n){
	char temp[12];
    temp[11] = '\0';
    int i = 10;
    
    if (n == 0) {
        temp[i--] = '0';
    } else {
        while (n > 0) {
            temp[i--] = '0' + (n % 10);
            n /= 10;
        }
    }
    
    
    int start = i + 1;
    int len = 11 - start;
    
    
    char *buffer = (char *)kmalloc(len + 1);
    if (buffer == NULL) return NULL;
    
   
    for (int j = 0; j < len; j++) {
        buffer[j] = temp[start + j];
    }
    buffer[len] = '\0';
    
    return buffer; 
}

static char *convert_pointer_to_char_arr(void *ptr){

	uint32_t value = (uint32_t)ptr;

        char hex_chars[] = "0123456789ABCDEF";
        char *buffer = (char *)kmalloc(12);

        buffer[0] = '0';
        buffer[1] = 'x';

        for (int i = 0; i < 8; i++){
                buffer[9 - i] = hex_chars[value & 0xF];  // Fill from right to left
                    value >>= 4;
        }
        buffer[10] = '\0';

	return buffer;
}


static uint32_t display_vga_text(char *buffer, uint32_t len, uint32_t temp_cursor_pos){

	for (uint32_t i = 0; i < len; i++){
                video_cursor[temp_cursor_pos * 2] = buffer[i];
                video_cursor[temp_cursor_pos * 2 + 1] = 0x0F;
                temp_cursor_pos++;
        }

	return temp_cursor_pos;
}


static void malloc_and_print(char *size){
	uint32_t converted_size = 12;//convert_char_to_int(size);

	if (converted_size == 0) return;
	void *addr = kmalloc(converted_size);
	uint32_t temp_cursor_pos = ((row * 2) - 1) * 80;
	temp_cursor_pos = display_vga_text("ALLOCATED!", 10, temp_cursor_pos);
}


uint32_t convert_char_to_int(char *str) {
    uint32_t result = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') {
            return 0; 
        }

        result = result * 10 + (str[i] - '0');
    }

    return result;
}
