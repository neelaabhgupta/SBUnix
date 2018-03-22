#ifndef _IO_FUNC_H
#define _IO_FUNC_H


void outb(unsigned char value, unsigned short int port);
unsigned char inb(unsigned short int port);
void outl(unsigned int value, unsigned short int port);
unsigned int inl(unsigned short int port);

#endif