#include<signal.h>

int kill(pid_t pid, int sig)
{
	uint64_t syscall_num = 16;
	__asm__ volatile("mov %0, %%r12;\n"
		"int $0x80;\n"
		:
		:"g"(syscall_num)
		);
	return 1;
}