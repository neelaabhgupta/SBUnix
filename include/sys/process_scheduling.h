#ifndef _PROCESS_SCHEDULING_H
#define _PROCESS_SCHEDULING_H
#include<sys/init_pd.h>
#include<sys/defs.h>
#include<sys/kprintf.h>
//#include<sys/sys_calls.h>
#include<sys/elf64.h>
//https://stackoverflow.com/questions/42653095/what-is-the-difference-between-retq-and-ret
//http://www.cs.columbia.edu/~krj/os/lectures/L17-LinuxPaging.pdf

int process_count;
struct task_struct* me;
uint64_t kernel_pml4_address;
struct process_list_node* process_list;
struct process_list_node* process_list_last;

uint64_t HEAP_ADDRESS;
uint64_t STACK_ADDRESS;
uint64_t KERNEL_STACK_ADDRESS;

#define STACK_LIMIT 3

struct vm_area_struct
{
	struct mm_struct* mmstruct;
	uint64_t vm_start;
	uint64_t vm_end;
	uint64_t paddress;
	struct vm_area_struct* vm_next;
};


struct mm_struct
{
	struct vm_area_struct* mmmap;
	uint64_t total_vm;	
};



struct task_struct
{	
	uint64_t rsp;	
	uint64_t *ustack;	
	uint64_t rdi;
	uint64_t cr3;
	uint64_t pid;
	uint64_t ppid;
	uint64_t rip;
	uint64_t kstack[512];
	uint64_t task_struct_state;
	struct mm_struct* mmstruct;
	struct task_struct* next;
	uint64_t heap_address;
	char current_directory[128];
	uint64_t arg_val;
	char arg_path[128];
};

struct process_list_node
{
	struct task_struct* task;
	struct process_list_node* next;
};

typedef struct task_struct* taskptr;


void init_process_list();
taskptr init_task();
void func_a();
void func_b();
void func_main();
void switch_to(taskptr me, taskptr next);
void LoadFirstProcess();
void ring0_ring3(uint64_t func);
extern void switch_to(taskptr me, taskptr next);
void schedule_for_a();
void schedule_for_b();
void yield();
void test_user_function();
void sbush();
void switch_ring3();
uint64_t get_rip();
void AllocateUserStackPage(taskptr task);
void AllocateKernelStackPage(taskptr task);
void CreateHeapVma(taskptr task);
void Traverse_process_PML4(uint64_t vaddress, uint64_t paddress, uint64_t *PML4_address, uint64_t page_entry_flag);
void Traverse_process_PDP(uint64_t vaddress, uint64_t paddress, uint64_t *PDP_address, uint64_t page_entry_flag);
void Traverse_process_PD(uint64_t vaddress, uint64_t paddress, uint64_t *PD_address, uint64_t page_entry_flag);
void Traverse_process_PT(uint64_t vaddress, uint64_t paddress, uint64_t *PT_address, uint64_t page_entry_flag);
uint64_t init_process_paging();
void init_process_count();
void init_process_list();
void Add_task_to_list(taskptr task1);
void Traverse_Process_list();
taskptr readelf(char* file, void (*func)());
uint64_t read_rip();
void readelf_1(char* file, taskptr task);
void ReinitialiseTask(char* file);
void CleanPageTableEntries(taskptr task);

void strcopy(char* source, char* destination);
//void 

#endif