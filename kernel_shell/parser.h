#ifndef PARSER_H
#define PARSER_H
#include <stdint.h>

char *parse_command(char buffer[]);
char *parse_arg(char buffer[], uint32_t start_index);


#endif
