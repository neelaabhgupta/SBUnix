#ifndef _TERMINAL_H
#define _TERMINAL_H
#include<sys/kprintf.h>
#include<sys/defs.h>

#define TERMINAL_BUFFER_CAPACITY 512
int BUFF_READY;

//int read_params

char buffer[TERMINAL_BUFFER_CAPACITY];
int buffer_index;

void init_terminal();
void stdout();
void buff_write(char c);
//uint64_t stdin();
void backspace();
void clear_buffer();




#endif