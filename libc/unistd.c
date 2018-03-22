#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
//#include<dirent.h>

pid_t fork()
{
	uint64_t syscall_num = 2;
	__asm__ volatile("mov %0, %%r12;\n"
		"int $0x80;\n"
		:
		:"g"(syscall_num)
		);
	uint64_t ret;
	__asm__ volatile("movq %%r13, %0\n"
		:"=r"(ret)
		:
		);
	return (pid_t)ret;
}

ssize_t write(int fd, const void *buf, size_t count)
{
	uint64_t syscall_num = 0;
	//char *string3 = "Child";
	__asm__ volatile("mov %0, %%r12;\n"
		"mov %1, %%r13;\n"
		"mov %2, %%r14;\n"
		"mov %3, %%r15;\n"
		"int $0x80;\n"
		:
		:"g"(syscall_num), "g"((uint64_t)fd), "g"((uint64_t)buf), "g"(count)
		);
	uint64_t ret;
	__asm__ volatile("movq %%r13, %0\n"
		:"=r"(ret) //g not working
		:
		);
	return (ssize_t)ret;
}

void yield()
{
	uint64_t syscall_num = 3;
	__asm__ volatile("mov %0, %%r12;\n"
		"int $0x80;\n"
		:
		:"g"(syscall_num)
		);
}

void ExecuteChildProcess(const char* file)
{
	uint64_t syscall_num = 7;
	//char *string3 = "Child";
	__asm__ volatile("mov %0, %%r12;\n"
		"mov %1, %%r13;\n"
		"int $0x80;\n"
		:
		:"g"(syscall_num), "g"((uint64_t)file)
		);
}


int execvpe(const char *file, char *const argv[], char *const envp[])
{
	pid_t pid = fork();
	if(pid == 0)
	{
		//write(1, file, 20);
		ExecuteChildProcess(file);
		//write(0, "Back in execvpe\n", 30);
		//while(1);
		//exit(1);
		//yield();
	}
	else
	{
		yield();
	}
	//char* string = "Back to parent\n";
	//write(1, string, 20);
	//exit(1);
	//write(1, string, 20);
	return pid;
}

void ps()
{
	uint64_t syscall_num = 9;
	__asm__ volatile("mov %0, %%r12;\n"
		"int $0x80;\n"
		:
		:"g"(syscall_num)
		);
}

unsigned int sleep(unsigned int seconds)
{
	uint64_t syscall_num = 10;
	__asm__ volatile("mov %0, %%r12;\n"
		"int $0x80;\n"
		:
		:"g"(syscall_num)
		);
	//return 1;
	uint64_t ret = 1000;
	__asm__ volatile("mov %%r13, %0;\n"
		:"=r"(ret)
		:
		);
	if(ret == 1)
	{
		return 1;
	}
	else return 0;
}

#if 1
ssize_t read(int fd, void *buf, size_t count)
{
	uint64_t ret = 5;
	uint64_t syscall_num = 6;
	__asm__ volatile("mov %0, %%r12;\n"
		"mov %1, %%r13;\n"
		"mov %2, %%r14;\n"
		"mov %3, %%r15;\n"
		"int $0x80;\n"
		:
		:"g"(syscall_num), "g"((uint64_t)fd), "g"((uint64_t)buf), "g"((uint64_t)count)
		);
	__asm__ volatile("movq %%r11, %0\n"
		:"=r"(ret)
		:
		);
	/*if(ret == 0)
	{
		write(1, "0", 3);
	}
	else 
	{
		write(1, "1", 3);
	}*/
	return (ssize_t)ret;
}
#endif

pid_t getpid(void)
{
	uint64_t syscall_num = 11;
	uint64_t ret = 50000;
	__asm__ volatile("mov %0, %%r12;\n"
		"int $0x80;\n"
		:
		:"g"(syscall_num)
		);
	__asm__ volatile("mov %%r11, %0;\n"
		"int $0x80;\n"
		:"=r"(ret)
		:
		);
	printf("pid == %d\n", ret);
	return (pid_t)ret;
}


pid_t getppid(void)
{
	uint64_t syscall_num = 12;
	uint64_t ret = 50000;
	__asm__ volatile("mov %0, %%r12;\n"
		"int $0x80;\n"
		:
		:"g"(syscall_num)
		);
	__asm__ volatile("mov %%r11, %0;\n"
		"int $0x80;\n"
		:"=r"(ret)
		:
		);
	printf("ppid == %d\n", ret);
	return (pid_t)ret;
}

void set_arg_val(uint64_t param1)
{
	uint64_t syscall_num = 13;
	__asm__ volatile("mov %0, %%r12;\n"
		"mov %1, %%r13;\n"
		"int $0x80;\n"
		:
		:"g"(syscall_num), "g"((uint64_t)param1)
		);
}

void set_arg_path(char* path)
{
	uint64_t syscall_num = 14;
	__asm__ volatile("mov %0, %%r12;\n"
		"mov %1, %%r13;\n"
		"int $0x80;\n"
		:
		:"g"(syscall_num), "g"((uint64_t)path)
		);
}

void echo()
{
	uint64_t syscall_num = 15;
	__asm__ volatile("mov %0, %%r12;\n"
		"int $0x80;\n"
		:
		:"g"(syscall_num)
		);
}

/*void kill()
{
	uint64_t syscall_num = 16;
	__asm__ volatile("mov %0, %%r12;\n"
		"int $0x80;\n"
		:
		:"g"(syscall_num)
		);
}*/

void clrscr()
{
	uint64_t syscall_num = 17;
	__asm__ volatile("mov %0, %%r12;\n"
		"int $0x80;\n"
		:
		:"g"(syscall_num)
		);
}

/*int chdir (const char *name){
	uint64_t syscall_num = 18;
	__asm__ volatile("mov %0, %%r12;\n"
		"int $0x80;\n"
		:
		:"g"(syscall_num)
		);
	uint64_t ret = 1000;
	__asm__ volatile("mov %%r13, %0"
		:"=r"(ret)
		:
		);
	int ret1;
	if(ret == 0)
	{
		ret1 = 0;
	}
	else
	{
		ret1 = -1;
	}
	return ret1;
}*/