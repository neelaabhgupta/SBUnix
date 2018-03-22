#ifndef _SYS_CALLS_H
#define _SYS_CALLS_H
#include<sys/defs.h>
#include<sys/process_scheduling.h>
//#include<sys/init_idt.h>

#define COW_BIT 0x0000000000000400

void write(uint64_t fd, char* buffer, uint64_t len);


typedef struct syscall_registers* syscall_regs;
void syscall();
uint64_t generate_syscall(uint64_t syscall_num, struct syscall_registers registers);
void fork();
void DeepCopyPageTable(taskptr child, taskptr parent);

void CopyVMA(taskptr child, taskptr me);

void sys_malloc(uint64_t size);
void sys_free(uint64_t vaddress);
void sys_yield();
uint64_t get_rip_val();

void ExecuteChild(char* file);
void exit();
void ps();
void sleep();
void sys_read(uint64_t param1, uint64_t param2, uint64_t param3);
void get_pid();
void get_ppid();
void set_arg_val(uint64_t param1);
void set_arg_path(char* path);
void echo();
void kill();

#endif