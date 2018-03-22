#include <stdlib.h>

void* malloc(size_t size)
{
	uint64_t syscall_num = 4;
	__asm__ volatile("mov %0, %%r12;\n"
		"mov %1, %%r13;\n"
		"int $0x80;\n"
		:
		:"g"(syscall_num), "g"((uint64_t)size)
		);
	uint64_t address;
	__asm__ volatile("movq %%r11, %0\n"
		:"=r"(address)
		:
		);
	return (void*)address;
}

void free(void *ptr)
{
	uint64_t syscall_num = 5;
	__asm__ volatile("mov %0, %%r12;\n"
		"mov %1, %%r13;\n"
		"int $0x80;\n"
		:
		:"g"(syscall_num), "g"((uint64_t)ptr)
		);
}

void exit(int status)
{
	uint64_t syscall_num = 8;
	__asm__ volatile("mov %0, %%r12;\n"
		"int $0x80;\n"
		:
		:"g"(syscall_num)
		);
}