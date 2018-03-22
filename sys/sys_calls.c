//http://www.jamesmolloy.co.uk/tutorial_html/9.-Multitasking.html
#include<sys/sys_calls.h>
#include<sys/kprintf.h>
#include<sys/gdt.h>
#include<sys/init_pd.h>
#include<sys/terminal.h>
#include<sys/isr_handler.h>

void syscall()
{
	uint64_t syscall_num;
	__asm__ volatile("movq %%r12, %0\n"
		:"=g"(syscall_num)
		:
		);

	if(syscall_num == 0)
	{
		uint64_t param1, param2, param3;
		__asm__ volatile("movq %%r13, %0\n"
			:"=g"(param1)
			:
			);
		__asm__ volatile("movq %%r14, %0\n"
			:"=g"(param2)
			:
			);
		__asm__ volatile("movq %%r15, %0\n"
			:"=g"(param3)
			:
			);
		write(param1, (char*)param2, param3);
	}
	else if(syscall_num == 2)
	{
		fork();
	}
	else if(syscall_num == 3)
	{
		sys_yield();
	}
	else if(syscall_num == 4)
	{
		//kprintf("Hey in  malloc!\n");
		uint64_t param1;
		__asm__ volatile("movq %%r13, %0\n"
			:"=g"(param1)
			:
			);
		sys_malloc(param1);
	}
	else if(syscall_num == 5)
	{
		uint64_t param1;
		__asm__ volatile("movq %%r13, %0\n"
			:"=g"(param1)
			:
			);
		sys_free(param1);
	}
	else if(syscall_num == 6)
	{
		//int fd, void *buf, size_t count
		uint64_t param1, param2, param3;
		__asm__ volatile("movq %%r13, %0\n"
			:"=g"(param1)
			:
			);
		__asm__ volatile("movq %%r14, %0\n"
			:"=g"(param2)
			:
			);
		__asm__ volatile("movq %%r15, %0\n"
			:"=g"(param3)
			:
			);
		sys_read(param1, param2, param3);
	}
	else if(syscall_num == 7)
	{
		uint64_t param1;
		__asm__ volatile("movq %%r13, %0\n"
			:"=g"(param1)
			:
			);
		ExecuteChild((char*)param1);
	}
	else if(syscall_num == 8)
	{
		exit();
	}
	else if(syscall_num == 9)
	{
		ps();
	}
	else if(syscall_num == 10)
	{
		sleep();
	}
	else if(syscall_num == 11)
	{
		get_pid();
	}
	else if(syscall_num == 12)
	{
		get_ppid();
	}
	else if(syscall_num == 13)
	{
		uint64_t param1;
		__asm__ volatile("movq %%r13, %0\n"
			:"=g"(param1)
			:
			);
		set_arg_val(param1);
	}
	else if(syscall_num == 14)
	{
		uint64_t param1;
		__asm__ volatile("movq %%r13, %0\n"
			:"=g"(param1)
			:
			);
		set_arg_path((char*)param1);
	}
	else if(syscall_num == 15)
	{
		echo();
	}
	else if(syscall_num == 16)
	{
		kill();
	}
	else if(syscall_num == 17)
	{
		clrscr();
	}
	//return ret;
}



void sys_read(uint64_t param1, uint64_t param2, uint64_t param3)
{
	//char command[TERMINAL_BUFFER_CAPACITY];
	char* command = (char *)param2;
	//kprintf("%x\n", command);
	if(BUFF_READY)
	{
		for(int i = 0;i < param3; i++)
		{
			if(buffer[i] == '\0')
			{
				break;
			}
			command[i] = buffer[i];
		}
		clear_buffer();
		uint64_t ret = 1;
		kprintf("SBUNIX:> ");
		__asm__ volatile("mov %0, %%r11;\n"
			:
			:"r"(ret)
			);
		
	}
	else
	{	
		uint64_t ret = 0;
		__asm__ volatile("mov %0, %%r11;\n"
			:
			:"r"(ret)
			);
	}
}

#if 0
uint64_t write(uint64_t fd, char* buffer, uint64_t len)
{
	/*kprintf("YOOOOOOOO!\n");
	uint64_t str;
	__asm__ volatile("mov %%r12, %0;\n"
		:"=g"(str)
		:
		);
	//kprintf("%s", (char*)str);
	//while(1);*/


	//kprintf("Yo");
	kprintf("The string is = %s\n", buffer);
	//kprintf("the fd is %d\n", fd);
	kprintf("the value of len is = %x\n", len);
	//kprintf("the value of pid = %d\n", me->pid);


	//while(1);


	return 1;
}
#endif

void write(uint64_t fd, char* buffer, uint64_t len)
{
	
	char* buff_print=(char*)kmalloc(4096);
	for(int i=0; i<len; i++){
		if(buffer[i] == '\0')
			break;
		else if(buffer[i] == (char)10)
		{
			 buff_print[i] = (char)10;
		}
		else buff_print[i]=buffer[i];
	}
	kprintf("%s",buff_print);
	__asm__ volatile("mov %0, %%r13\n"
		:
		:"r"((uint64_t)len)
		);
	//return len;
}




uint64_t get_rip_val()  
{
	uint64_t rip;
	__asm__ volatile("popq %%rax;\n"
		"jmp %%rax;\n"
		:"=g"(rip)
		:
		);
	return rip;

}

void fork()
{
	//kprintf("here in fork yayay!!\n");
	taskptr child = init_task(); 	
	child->rsp = me->rsp;
	child->rdi = me->rdi;
	child->ppid = me->pid;
	child->arg_val= me->arg_val;
	for(int i = 0; i < 128; i++)
	{
		if(me->arg_path[i] == '\0')
		{
			child->arg_path[i] = '\0';
			break;
		}
		child->arg_path[i] = me->arg_path[i];
	}
	child->cr3 = init_process_paging();	
	child->heap_address = me->heap_address;
	child->mmstruct = (struct mm_struct*)kmalloc((int)sizeof(struct mm_struct));
	//kprintf("The address of child mmstruct = %x\n", child->mmstruct);
	//kprintf("The total vm in parent = %x\n", me->mmstruct->total_vm);
	child->mmstruct->total_vm = me->mmstruct->total_vm;
	child->mmstruct->mmmap = NULL;
	CopyVMA(child, me);	
	DeepCopyPageTable(child, me);

	//kprintf("After Copy in Fork");

	//Copy Kernel Stack
	for(int i=0; i < 512; i++)
	{
		child->kstack[i] = me->kstack[i]; 
	}

	//Add child to list
	taskptr tempptr = me->next;	
	child->next = tempptr;
	me->next = child;
	//Add_task_to_list(child);

	taskptr parent = me;
	//
	uint64_t process_pid = 0;
	
	uint64_t rip = get_rip_val();
	if(parent == me)
	{
		//Save the rip in the top of child stack. Will be popped when ret is called in switch_to
		//kprintf("KERNEL LOG::::In condition 1\n");
		uint64_t rsp;
		__asm__ volatile("movq %%rsp, %0;\n"
			:"=g"(rsp)
			:
			);
		child->kstack[512 - (uint64_t)((uint64_t)parent->kstack + 4088 - rsp)/8] = rip;		
		child->rsp = (uint64_t)&(child->kstack[512 - (uint64_t)((uint64_t)parent->kstack + 4088 - rsp)/8]);
		process_pid = child->pid;
		child->rip = rip;
		__asm__ volatile("mov %0, %%r13\n"
			:
			:"r"((uint64_t)process_pid)
			);
	}
	else
	{
		//kprintf("KERNEL LOG::::In condition 2\n");
		//Add_task_to_list(parent);
		//kprintf("parent pid in condition 2 = %d", parent->pid);
		/*if(parent == NULL)
		{
			kprintf("Parent is NULL now\n");
		}*/
		/*if(me->next == NULL)
		{
			kprintf("Parent is NULL now\n");
		}*/
		process_pid = 0;
		__asm__ volatile("mov %0, %%r13\n"
			:
			:"r"((uint64_t)process_pid)
			);
	}
}

void CopyVMA(taskptr child, taskptr me)
{
	//copy vm_area_structs
	struct vm_area_struct* child_parent = NULL;
	struct vm_area_struct* parentvm = me->mmstruct->mmmap;
	for(int i = 0; i < me->mmstruct->total_vm; i++)
	{
		if(child->mmstruct->mmmap == NULL)
		{
			child->mmstruct->mmmap = (struct vm_area_struct*)kmalloc((int)sizeof(struct vm_area_struct));
			child->mmstruct->mmmap->vm_start = me->mmstruct->mmmap->vm_start;
			child->mmstruct->mmmap->vm_end = me->mmstruct->mmmap->vm_end;
			child->mmstruct->mmmap->mmstruct = (struct mm_struct*)child->mmstruct;
			//child->mmstruct->mmmap->vm_flags = me->mmstruct->mmmap->vm_flags;
			child->mmstruct->mmmap->vm_next = NULL;
			child_parent = (struct vm_area_struct*)child->mmstruct->mmmap;
			parentvm = (struct vm_area_struct*)me->mmstruct->mmmap;
		}
		else
		{
			child_parent->vm_next = (struct vm_area_struct*)kmalloc((int)sizeof(struct vm_area_struct));
			child_parent->vm_next->vm_start = parentvm->vm_next->vm_start;
			child_parent->vm_next->vm_end = parentvm->vm_next->vm_end;
			child_parent->vm_next->mmstruct = (struct mm_struct*)child->mmstruct;
			//child_parent->vm_next->vm_flags = parent->vm_next->vm_flags;
			child_parent->vm_next->vm_next = NULL;
			child_parent = (struct vm_area_struct*)child_parent->vm_next;
			parentvm = (struct vm_area_struct*)parentvm->vm_next;
		}
	}
}

void DeepCopyPageTable(taskptr child, taskptr parent)
{
	//kprintf("In Deep Copy\n");
	uint64_t* childcr3 = (uint64_t*)(child->cr3 + mapping_mask);
	uint64_t* parentcr3 = (uint64_t*)(parent->cr3 + mapping_mask);
	//kprintf("Child = %x\n", childcr3);
	//kprintf("parentcr3 = %x\n", parentcr3);
	for(int i = 0; i < 511; i++) //Since Kernel's page tables are already mapped
	{
		if(parentcr3[i]!=0)
		{
			//kprintf("here1\n");
			//kprintf("i = %d\n", i);
			uint64_t* child_pdp = (uint64_t*)kmalloc(4096);
			uint64_t* parent_pdp = (uint64_t*)((parentcr3[i] & 0xFFFFFFFFFFFFF000) + mapping_mask);
			for(int j = 0; j < 512 ; j++)
			{
				//kprintf("j = %d\n", j);
				if(parent_pdp[j] != 0)
				{
					//kprintf("here2\n");
					uint64_t* child_pd = (uint64_t*)kmalloc(4096);
					uint64_t* parent_pd = (uint64_t*)((parent_pdp[j] & 0xFFFFFFFFFFFFF000) + mapping_mask);
					for(int k = 0; k < 512; k++)
					{
						//kprintf("k = %d\n", k);
						if(parent_pd[k] != 0)
						{
							//kprintf("here3\n");
							uint64_t* child_pt = (uint64_t*)kmalloc(4096);
							uint64_t* parent_pt = (uint64_t*)((parent_pd[k] & 0xFFFFFFFFFFFFF000) + mapping_mask);
							for(int l = 0; l < 512; l++)
							{
								//kprintf("l = %d\n", l);
								if(parent_pt[l] != 0)
								{
									//kprintf("here4\n");
									uint64_t page_address = parent_pt[l] & 0xFFFFFFFFFFFFF000;
									uint64_t page_num = (page_address - first_page_address)/4096;
									//kprintf("Page number = %x\n", page_num);
									uint64_t node_address = free_list_start + page_num*sizeof(struct FreeListNode);
									freelistnode node = (freelistnode)(node_address + mapping_mask);
									node->page_usage_count++;
									//kprintf("The parent address = %x   Node- page address = %x\n", (parent_pt[l] & 0xFFFFFFFFFFFFF000), node->address);
									child_pt[l] = (parent_pt[l] & 0xFFFFFFFFFFFFFFFD) | COW_BIT | 0x0000000000000005;
									parent_pt[l] = (parent_pt[l] & 0xFFFFFFFFFFFFFFFD) | COW_BIT | 0x0000000000000005;
									//child_pt[l] = parent_pt[l] | 0x0000000000000007;
								}
							}
							child_pd[k] = (((uint64_t)child_pt - mapping_mask) | 0x0000000000000007);
						}
					}
					child_pdp[j] = (((uint64_t)child_pd - mapping_mask) | 0x0000000000000007);
				}
			}
			childcr3[i] = (((uint64_t)child_pdp - mapping_mask) | 0x0000000000000007);
		}
	}
	updateCR3(parent->cr3);
}

void sys_yield()
{
	taskptr last = me;
	//kprintf(" sys_yield Value of pid before = %d\n", last->pid);
	if(me->next == NULL)
	{
		kprintf("KERNEL LOG:::Only one current Process\n");
		//kprintf("sys_yield pid in me = %x\n", me->pid);
		/*uint64_t cr3 = 0;
		__asm__ volatile("movq %%cr3, %0;\n"
		:"=r"((uint64_t)cr3)
		:
		);*/
		//kprintf("Current cr3 = %x\n", cr3);
		//kprintf("cr3 addres of b = %x\n", me->cr3);
		return;
	}
	//kprintf("pid in me = %x\n", me->pid);
	me = me->next;
	//Add Last to the list
	taskptr temp = me->next;
	me->next = last;
	last->next = temp;
	//kprintf("sys_yield Value of pid after = %d\n", me->pid);
	//kprintf("pid in me->next = %x\n", me->pid);
	//kprintf("cr3 addres of b = %x\n", me->cr3);
	//updateCR3(me->cr3);
	//
	
	__asm__ volatile("movq %0, %%cr3;"
		:
		:"r"((uint64_t)me->cr3)
		);
	set_tss_rsp(&(me->kstack[511]));
	//updateCR3(me->cr3);
	//kprintf("Cr3 switching done!\n");
	//uint64_t a = kmalloc(4096);
	//kprintf("The address is = %x\n", a);
	switch_to(last, me);
	//yield();
	return;
	
	#if 0
	taskptr last = me;
	taskptr next = process_list->next->task;
	if(next == NULL)
	{
		kprintf("No new process to schedule\n");
		return;
	}
	else
	{
		
		process_list = process_list->next;
		me = next;
		kprintf("In sys_yield\n");
		kprintf("The pid value = %d\n", me->pid);
		Add_task_to_list(last);
		__asm__ volatile("movq %0, %%cr3;"
		:
		:"r"((uint64_t)me->cr3)
		);
		set_tss_rsp(&(me->kstack[511]));
		kprintf("The value of pid in me = %d\n", me->pid);
		kprintf("The value of pid in last = %d\n", last->pid);
		switch_to(last, me);
		return;
	}
	#endif
	
	//while(1);
}

void sys_malloc(uint64_t size)
{
	uint64_t page = kmalloc(size);
	taskptr current = me;
	struct vm_area_struct* parent = current->mmstruct->mmmap;
	for(int i = 0; i < current->mmstruct->total_vm - 1; i++)
	{
		parent = parent->vm_next;
	}
	if(parent == NULL)
	{
		parent = (struct vm_area_struct*)kmalloc(sizeof(struct vm_area_struct));
		parent->vm_start = current->heap_address;
		parent->vm_end = current->heap_address + size;
		parent->mmstruct = current->mmstruct;
		parent->paddress = (page & 0xFFFFFFFFFFFFF000) - mapping_mask;
		parent->vm_next = NULL;
	}
	else
	{
		parent->vm_next = (struct vm_area_struct*)kmalloc(sizeof(struct vm_area_struct));
		parent->vm_next->vm_start = current->heap_address;
		parent->vm_next->vm_end = current->heap_address + size;
		parent->vm_next->mmstruct = current->mmstruct;
		parent->vm_next->paddress = (page & 0xFFFFFFFFFFFFF000) - mapping_mask;
		parent->vm_next->vm_next = NULL;
	}
	current->mmstruct->total_vm++;
	int s = size;
	Traverse_process_PML4(current->heap_address, (page & 0xFFFFFFFFFFFFF000) - mapping_mask, (uint64_t*)get_virtual_address(current->cr3,0), 0x0000000000000007);
	s = s - 4096;
	int i = 1;
	while(s>0)
	{
		Traverse_process_PML4(current->heap_address + i*4096, (page & 0xFFFFFFFFFFFFF000) + i*4096 - mapping_mask, (uint64_t*)get_virtual_address(current->cr3,0), 0x0000000000000007);
		i++;
		s = s - 4096;
	}
	uint64_t current_addr = current->heap_address;
	//kprintf("Current Address in malloc = %x\n", current->heap_address);
	__asm__ volatile("mov %0, %%r11\n"
		:
		:"r"(current_addr)
		);
	//kprintf("Current Heap Address = %x\n", current->heap_address);
	//kprintf("Current Physical Address = %x\n", parent->vm_next->paddress);
	current->heap_address = current->heap_address + size;
	updateCR3(current->cr3);
	//vm->vm_end = vm->vm_end + size;
	
	//kprintf("Malloc end\n");
}

void sys_free(uint64_t vaddress)
{
	taskptr current = me;
	struct vm_area_struct* parent = current->mmstruct->mmmap;
	if(parent == NULL)
	{
		kprintf("KERNEL LOG:::No Vma to remove!\n");
	}
	else
	{
		int flag = 0;
		for(int i = 0; i < current->mmstruct->total_vm - 1; i++)
		{
			if(parent->vm_next != NULL && parent->vm_next->vm_start == vaddress)
			{
				flag =1;
				break;
			}
			else parent = parent->vm_next;
		}
		if(flag)
		{
			struct vm_area_struct* vma_to_free = parent->vm_next;
			int size = vma_to_free->vm_end - vma_to_free->vm_start;
			freepage(vma_to_free->paddress);
			Traverse_process_PML4(vaddress, vma_to_free->paddress, (uint64_t*)get_virtual_address(current->cr3, 0), 0x0);
			size = size - 4096;
			int i = 1;
			while(size > 0)
			{
				freepage(vma_to_free->paddress + i*4096);
				Traverse_process_PML4(vaddress + i*4096, vma_to_free->paddress + i*4096, (uint64_t*)get_virtual_address(current->cr3, 0), 0x0);
				size = size - 4096;
			}
			parent->vm_next = vma_to_free->vm_next;
			freepage((uint64_t)vma_to_free - mapping_mask);
			current->mmstruct->total_vm--;
			updateCR3(current->cr3);
		}
		else
		{
			kprintf("KERNEL LOG:::Memory Has already been freed!\n");
		}
	}
	
}

void ExecuteChild(char* file)
{
	ReinitialiseTask(file);
}

void exit()
{
	//same as yield
	taskptr last = me;
	if(me->next == NULL)
	{
		kprintf("KERNEL LOG:::Cannot Exit. Os will Shut Down. Only one current Process\n");
		return;
	}
	//kprintf("pid in me = %x\n", me->pid);
	me = me->next;

	#if 0
	//Add Last to the list
	taskptr temp = me->next;
	me->next = last;
	last->next = temp;
	#endif
	struct vm_area_struct *vma = last->mmstruct->mmmap;
	struct vm_area_struct *temp = NULL;
	for(int i = 0; i < last->mmstruct->total_vm; i++)
	{
		if(vma)
		{
			temp = vma;
			vma = vma->vm_next;
			freepage((uint64_t)temp - mapping_mask);
		}
	}
	last->mmstruct->mmmap = NULL;
	last->mmstruct->total_vm = 0;
	CleanPageTableEntries(last);
	__asm__ volatile("movq %0, %%cr3;"   //TODO: updateCR3 function not working.
		:
		:"r"((uint64_t)me->cr3)
		);
	freepage(last->cr3);
	int size = sizeof(struct task_struct);
	freepage((uint64_t)last - mapping_mask);
	size = size - 4096;
	int i = 1;
	while(size > 0)
	{
		freepage((uint64_t)last - mapping_mask + i*4096);
		i++;
		size = size - 4096;
	}
	set_tss_rsp(&(me->kstack[511]));
	//updateCR3(me->cr3);
	//kprintf("Cr3 switching done!\n");
	//uint64_t a = kmalloc(4096);
	//kprintf("The address is = %x\n", a);
	//switch_to(last, me);
	__asm__ volatile("mov %0, %%rsp;\n"
		"ret"
		:
		:"g"(me->rsp)
		);
	//yield();
	return;
}


void ps()
{
	for(int i = 0; i < 9;i++)
	{
		kprintf_bsp();
	}
	taskptr current = me;
	kprintf("\n");
	int i = 1;
	kprintf("S.No.			Process ID\n");
	while(current != NULL)
	{		
		kprintf("%d 			     %d\n", i, current->pid);
		i++;
		current = current->next;
	}
}

void sleep()
{
	//set_sleep_flag();
/*	for(int i = 0; i < 9;i++)
	{
		kprintf_bsp();
	}*/
	if(sleepf == 0)
	{
		timer_capacity = me->arg_val*18;
		//kprintf("Timer Capacity = %d\n", timer_capacity);
		is_sleep = 1;
		sleep_flag = 0;
		sleepf = 1;
	}
	/*while(is_sleep == 1)
	{
		kprintf("%d\n", is_sleep);
	}*/
	uint64_t ret;
	if(is_sleep)
	{
		ret = 0;
		//do nothing
		__asm__ volatile("mov %0, %%r13;\n"
			:
			:"r"(ret)
			);
	}
	else
	{
		is_sleep = 0;
		sleep_flag = 0;
		timer_capacity = 0;
		sleepf = 0;
		ret = 1;
		__asm__ volatile("mov %0, %%r13;\n"
			:
			:"r"(ret)
			);
	}
	
}

void get_pid()
{
	__asm__ volatile("mov %0, %%r11;\n"
		:
		:"r"((uint64_t)me->pid)
		);
}

void get_ppid()
{
	__asm__ volatile("mov %0, %%r11;\n"
		:
		:"r"((uint64_t)me->ppid)
		);
}

void set_arg_val(uint64_t param1)
{
	me->arg_val = param1;
}

void set_arg_path(char* path)
{
	for(int i = 0; i < 128; i++)
	{
		if(path[i] == '\0')
		{
			me->arg_path[i] = '\0';
			break;
		}
		me->arg_path[i] = path[i];
	}
}

void echo()
{
	for(int i = 0; i < 9;i++)
	{
		kprintf_bsp();
	}
	kprintf("%s\n", me->arg_path);
}

void kill()
{
	taskptr current = me;
	taskptr task_to_kill = NULL;
	taskptr prev = NULL;
	while(current != NULL)
	{
		if(current->pid == me->arg_val)
		{
			task_to_kill = current;
			//if(prev!=NULL)
				//prev->next = task_to_kill->next;
			break;
		}
		prev = current;
		current = current->next;		
	}
	if(task_to_kill == NULL)
	{
		kprintf("KERNEL LOG:::Process with pid [%d] not found.\n", me->arg_val);
	}
	else if(task_to_kill == me && task_to_kill->next == NULL)
	{
		kprintf("KERNEL LOG:::Only one process running. OS will shut down. Cannot Kill!\n");
	}
	else if(task_to_kill == me && task_to_kill->next != NULL)
	{
		kprintf("\n\n KERNEL LOG:::Commiting Suicide!\n\n");
		exit();
	}
	else
	{
		taskptr last = task_to_kill;
		
		//Check if task_to_kill is sbush or not
		if(task_to_kill->pid == 1)
		{
			kprintf("KERNEL LOG:::Cannot Kill Sbush\n");
			return;
		}
		prev->next = task_to_kill->next;
		// if(me->next == NULL)
		// {
		// 	kprintf("KERNEL LOG:::Cannot Exit. Os will Shut Down. Only one current Process\n");
		// 	return;
		// }
		//kprintf("pid in me = %x\n", me->pid);
		//me = me->next;

		#if 0
		//Add Last to the list
		taskptr temp = me->next;
		me->next = last;
		last->next = temp;
		#endif
		struct vm_area_struct *vma = last->mmstruct->mmmap;
		struct vm_area_struct *temp = NULL;
		for(int i = 0; i < last->mmstruct->total_vm; i++)
		{
			if(vma)
			{
				temp = vma;
				vma = vma->vm_next;
				freepage((uint64_t)temp - mapping_mask);
			}
		}
		last->mmstruct->mmmap = NULL;
		last->mmstruct->total_vm = 0;
		CleanPageTableEntries(last);
		/*__asm__ volatile("movq %0, %%cr3;"   //TODO: updateCR3 function not working.
			:
			:"r"((uint64_t)me->cr3)
			);*/
		freepage(last->cr3);
		int size = sizeof(struct task_struct);
		freepage((uint64_t)last - mapping_mask);
		size = size - 4096;
		int i = 1;
		while(size > 0)
		{
			freepage((uint64_t)last - mapping_mask + i*4096);
			i++;
			size = size - 4096;
		}
		//set_tss_rsp(&(me->kstack[511]));
		//updateCR3(me->cr3);
		//kprintf("Cr3 switching done!\n");
		//uint64_t a = kmalloc(4096);
		//kprintf("The address is = %x\n", a);
		//switch_to(last, me);
		/*__asm__ volatile("mov %0, %%rsp;\n"
			"ret"
			:
			:"g"(me->rsp)
			);*/
	}
	return;
}

