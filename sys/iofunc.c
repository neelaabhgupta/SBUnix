#include<sys/iofunc.h>
#include<sys/kprintf.h>



//Citation - http://wiki.osdev.org/Inline_Assembly/Examples#I.2FO_access
//http://man7.org/linux/man-pages/man2/inl.2.html

void outb(unsigned char value, unsigned short int port)
{
	__asm__ volatile("outb %0, %1\n"
		:
		:"a"(value), "Nd"(port)
		);
}

unsigned char inb(unsigned short int port)
{
	unsigned char mask;
	__asm__ volatile("inb %1, %0\n"
		:"=a"(mask)
		:"Nd"(port)
		);
	return mask;
}

void outl(unsigned int value, unsigned short int port)
{
	__asm__ volatile("outl %0, %1\n"
		:
		:"a"(value), "d"(port)
		);
}

unsigned int inl(unsigned short int port)
{
	unsigned int mask;
	__asm__ volatile("inl %1, %0\n"
		:"=a"(mask)
		:"d"(port)
		);
	return mask;
}