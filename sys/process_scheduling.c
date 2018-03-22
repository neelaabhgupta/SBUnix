#include<sys/process_scheduling.h>
#include<sys/init_pd.h>
#include<sys/gdt.h>
#include<sys/tarfs.h>

//static taskptr next;

uint64_t HEAP_ADDRESS = 0x9000000000;
uint64_t STACK_ADDRESS = 0x70000000;
uint64_t KERNEL_STACK_ADDRESS = 0x8000000;

void init_process_count()
{
	process_count = 0;
}

taskptr init_task()
{
	taskptr task = (taskptr)kmalloc(sizeof(struct task_struct));
	task->rdi = 0;
	task->pid = ++process_count;
	//kprintf("Process id = %d\n", task->pid);
	task->mmstruct = NULL;
	task->next = NULL;
	//task->
	task->heap_address = HEAP_ADDRESS;
	task->arg_val = 0;
	for(int i = 0; i<512; i++)
	{
		task->arg_path[i] = '\0';
	}
	return task;
}

void func_a()
{
	__asm__ volatile("movq %0, %%cr3;"
		:
		:"r"((uint64_t)me->cr3)
		);
	set_tss_rsp(&(me->kstack[511]));
	__asm__ volatile("movq %0, %%rsp\n"
		:
		:"r"((uint64_t)me->ustack + 511*8)
		);
	ring0_ring3((uint64_t)me);
}


void func_b()
{

	//kprintf("Hello from Func B\n");
	#if 0
	yield();
	kprintf("calling A again\n");
	yield();
	kprintf("Bye from B\n");
	#endif
	//kprintf("value of pid in me func_b= %d\n", me->pid);
	//while(1);
	
	updateCR3(me->cr3);
	set_tss_rsp(&(me->kstack[511]));
	__asm__ volatile("movq %0, %%rsp\n"
		:
		:"g"((uint64_t)me->ustack + 511*8)
		);
	ring0_ring3((uint64_t)me);

	//switch_to(last, me);
}

void func_main()
{
	kprintf("hello from main\n");
}





void LoadFirstProcess()
{
	//save pml4 address
	uint64_t pml4_address;
	__asm__ volatile("movq %%cr3, %0\n"
		:"=g"(pml4_address)
		:
		);
	kernel_pml4_address = pml4_address;
	init_process_count();

	//Create one processes
	taskptr task_a = (taskptr)readelf("bin/sbush", sbush);
	//kprintf("After ELF\n");
	AllocateUserStackPage(task_a);
	
	task_a->rsp = (uint64_t)(&(task_a->kstack[511]));
	uint64_t func_page_address = (uint64_t)&switch_to & 0xFFFFFFFFFFFFF000;
	set_page_flags(func_page_address, 2, mapping_mask);
	me = task_a;
	init_process_list(task_a);
	strcopy("/", task_a->current_directory);
	//kprintf("The current directory is = %s\n", task_a->current_directory);
	//Switch to kernel Stack of the process....
	__asm__ volatile("movq %0, %%rsp;\n" 
		:
		:"g"(task_a->rsp)
		);
	__asm__ volatile("movq %0, %%cr3;"
		:
		:"r"((uint64_t)me->cr3)
		);
	set_tss_rsp(&(me->kstack[511]));
	__asm__ volatile("movq %0, %%rsp\n"
		:
		:"r"((uint64_t)me->ustack + 511*8)
		);
	ring0_ring3((uint64_t)me);

}

uint64_t read_rip()  //use this also
{
	uint64_t rip;
	__asm__ volatile("popq %%rax;\n"
		"jmp %%rax;\n"
		:"=g"(rip)
		:
		);
	return rip;
}

void yield()
{
	/*taskptr last = me;
	me = me->next;
	switch_to(last, me);*/

	taskptr last = me;
	/*me = process_list->task;
	process_list = process_list->next;*/
	me = me->next;
	updateCR3(me->cr3);
	set_tss_rsp((uint64_t*)&me->kstack[511]);
	switch_to(last, me);
}

void switch_ring3()
{
	//kprintf("Address of test_user_function = %x\n", (uint64_t)&test_user_function);
	//kprintf("Address of ring0_ring3 = %x\n", (uint64_t)&ring0_ring3);
	uint64_t func_page_address;
	
	
	uint64_t pml4_address;
	__asm__ volatile("movq %%cr3, %0\n"
		:"=g"(pml4_address)
		:
		);
	kernel_pml4_address = pml4_address;


	func_page_address = (uint64_t)&test_user_function & 0xFFFFFFFFFFFFF000;
	set_page_flags(func_page_address, 2, mapping_mask);
	
	taskptr task = (taskptr)readelf("bin/sbush", sbush);
	//kprintf("After ELF\n");
	AllocateUserStackPage(task);
	//AllocateKernelStackPage(task);
	
	//kprintf("After stack func\n");
	
	//kprintf("Address of new cr3 = %x\n", task->cr3);
	//kprintf("Value in task->kstack = %x\n", task->ustack);
	//kprintf("Value in kstack = %x\n", *(task->ustack));
	
	updateCR3(task->cr3);
	set_tss_rsp((&task->kstack[511]));
	
	__asm__ volatile("movq %0, %%rsp\n"
		:
		:"g"((uint64_t)task->ustack + 511*8)
		);
	me = task;
	me->next = NULL;
	/*int *a = (int*)0x6FFFFFF;
 	*a = 12;*/
	ring0_ring3((uint64_t)task);
	//ring0_ring3((uint64_t)&test_user_function);
	
	//while(1);
}

void AllocateUserStackPage(taskptr task)
{
	uint64_t addr = (uint64_t)kmalloc(4096);
	//kprintf("Address of addr = %x\n", addr);
	Traverse_process_PML4(STACK_ADDRESS, addr - mapping_mask, (uint64_t*)get_virtual_address(task->cr3, 0), 0x0000000000000007);
	task->ustack = (uint64_t*)STACK_ADDRESS;
	struct vm_area_struct* parent = task->mmstruct->mmmap;
	for(int i = 0; i < task->mmstruct->total_vm - 1; i++)
	{
		parent = parent->vm_next;
	}
	if(parent == NULL)
	{
		parent = (struct vm_area_struct*)kmalloc(sizeof(struct vm_area_struct));
		parent->vm_start = STACK_ADDRESS + 4096;
		parent->vm_end = STACK_ADDRESS;
		parent->mmstruct = task->mmstruct;
		parent->vm_next = NULL;
	}
	else
	{
		parent->vm_next = (struct vm_area_struct*)kmalloc(sizeof(struct vm_area_struct));
		parent->vm_next->vm_start = STACK_ADDRESS + 4096;
		parent->vm_next->vm_end = STACK_ADDRESS;
		parent->vm_next->mmstruct = task->mmstruct;
		parent->vm_next->vm_next = NULL;
	}
	task->mmstruct->total_vm++;
}

void CreateHeapVma(taskptr task)
{
	/*uint64_t addr = (uint64_t)kmalloc(4096);
	kprintf("Address of addr = %x\n", addr);
	Traverse_process_PML4(STACK_ADDRESS, addr - mapping_mask, (uint64_t*)get_virtual_address(task->cr3, 0), 0x0000000000000007);
	task->ustack = (uint64_t*)STACK_ADDRESS;*/
	struct vm_area_struct* parent = task->mmstruct->mmmap;
	for(int i = 0; i < task->mmstruct->total_vm - 1; i++)
	{
		parent = parent->vm_next;
	}
	if(parent == NULL)
	{
		parent = (struct vm_area_struct*)kmalloc(sizeof(struct vm_area_struct));
		parent->vm_start = HEAP_ADDRESS;
		parent->vm_end = HEAP_ADDRESS;
		parent->mmstruct = task->mmstruct;
		parent->vm_next = NULL;
	}
	else
	{
		parent->vm_next = (struct vm_area_struct*)kmalloc(sizeof(struct vm_area_struct));
		parent->vm_start = HEAP_ADDRESS;
		parent->vm_end = HEAP_ADDRESS;
		parent->vm_next->mmstruct = task->mmstruct;
		parent->vm_next->vm_next = NULL;
	}
	task->mmstruct->total_vm++;
}

void AllocateKernelStackPage(taskptr task)
{
	#if 0
	hikhnk
	#endif
}

void test_user_function()
{
	//kprintf("Hello");
	/*__asm__ volatile("int $0x80"
		:
		:
		);*/
	/*char *s = "Printing in user mode\n";
	struct syscall_registers regs;
	regs.num_params = 3;
	regs.param1 = 0;
	regs.param2 = (uint64_t)s;
	regs.param3 = 10;
	generate_syscall(0, regs);*/

	/*//kprintf("Value of rdi = %s\n", regs.rdi);
	//kprintf("Value of num_params = %d\n", regs.num_params);
	//syscall_regs regs1= (syscall_regs)&regs;
	//kprintf("address of regs1 = %x\n", regs1);
	//kprintf("value of address of (uint64_t)&regs = %x\n", (uint64_t)&regs);
	uint64_t syscall_num = 0;
	//uint64_t* syscall_num1 = &syscall_num;
	//kprintf("Value of syscall_num1 = %d\n", *syscall_num1);
	//kprintf("Address of syscall_num = %x\n", &syscall_num);
	uint64_t ret = 0;
	//generate_syscall(0, &regs);
	//r13
	//r11
	__asm__ volatile("mov %1, %%r12;\n"						
			"mov %2, %%r13;\n"
			"int $0x80;\n"
			:"=g"(ret)
			:"g"(syscall_num), "g"(regs.rdi)
			);	*/
	//kprintf("Hello"); 
	while(1);
	//return;
}

taskptr readelf(char* file, void (*func)())
{
	Elf64_Ehdr* eheader = (Elf64_Ehdr*)find_executable(file);
	if(eheader->e_ident[0] == 0x7f && eheader->e_ident[1] == 'E' && eheader->e_ident[2] == 'L' && eheader->e_ident[3] == 'F' && eheader->e_type == 2)
	{
		kprintf("");
		//;
	}
	taskptr task = init_task();
	task->mmstruct = (struct mm_struct*)kmalloc((int)sizeof(struct mm_struct));
	//kprintf("address of task mmstruct = %x\n", task->mmstruct);
	//task->rsp = (uint64_t)&task->kstack[511];

	//my_memset((void*)task->kstack, 0, 512*8);

	//task->kstack[511] = (uint64_t)eheader->e_entry;
	task->rip = (uint64_t)eheader->e_entry;
	task->mmstruct->mmmap = NULL;
	uint64_t process_pml4 = init_process_paging();
	task->cr3 = process_pml4;
	set_page_flags(process_pml4 & 0xFFFFFFFFFFFFF000, 2, mapping_mask);	
	//uint64_t process_entry_point = eheader->e_entry;
	//kprintf("process_entry_point = %x\n", process_entry_point);
	uint64_t program_header_table_address = (uint64_t)eheader + eheader->e_phoff;
	//kprintf("program_header_table_address = %x\n", program_header_table_address);
	uint64_t program_header_table_entries = eheader->e_phnum;
	//kprintf("program_header_table_entries = %x\n", program_header_table_entries);
	uint64_t program_header_table_entry_size = eheader->e_phentsize;
	struct vm_area_struct* parent = NULL;
	//task->mmstruct->mmmmap->total_vm = 0;
	for(uint64_t i = 0; i < program_header_table_entries; i++)
	{
		Elf64_Phdr* pheader = (Elf64_Phdr*)(program_header_table_address + i*program_header_table_entry_size);
		if(pheader->p_type == 1)
		{
			//Loadable section found
			//struct vm_area_struct* temp = NULL;
			//struct vm_area_struct* tempnext = task_sbush->mmstruct->mmmap;
			//kprintf("eheader address = %x, i = %d\n", eheader, i);

			if(task->mmstruct->mmmap == NULL)
			{
				uint64_t addr = memcopy((uint64_t)eheader + pheader->p_offset, (uint64_t)eheader + pheader->p_offset + pheader->p_filesz);
				//kprintf("Address of page = %x\n", addr);
				for(uint64_t i = 0; i <= pheader->p_filesz; i=i+4*1024)
				{
					Traverse_process_PML4(pheader->p_vaddr + i, (addr & 0x000000000fffffff) + i, (uint64_t*)get_virtual_address(process_pml4, 0), 0x0000000000000007);
					//kprintf("Address of page = %x\n", (addr & 0x000000000fffffff));
					//kprintf("Virtual Address of pageheader = %x\n", pheader->p_vaddr + i);
				}

				task->mmstruct->mmmap = (struct vm_area_struct*)kmalloc((int)sizeof(struct vm_area_struct));
				task->mmstruct->mmmap->vm_start = pheader->p_vaddr;
				task->mmstruct->mmmap->vm_end = pheader->p_vaddr + pheader->p_filesz;
				task->mmstruct->mmmap->mmstruct = (struct mm_struct*)task->mmstruct;
				//task->mmstruct->mmmap->vm_flags = pheader->p_flags;
				task->mmstruct->mmmap->vm_next = NULL;
				parent = (struct vm_area_struct*)task->mmstruct->mmmap;
				task->mmstruct->total_vm = 1;
			}
			else
			{
				//to be written
				uint64_t addr = memcopy((uint64_t)eheader + pheader->p_offset, (uint64_t)eheader + pheader->p_offset + pheader->p_filesz);
				//kprintf("Address of page = %x\n", addr);
				for(uint64_t i = 0; i <= pheader->p_filesz; i=i+4*1024)
				{
					Traverse_process_PML4(pheader->p_vaddr + i, (addr & 0x000000000fffffff) + i, (uint64_t*)get_virtual_address(process_pml4, 0), 0x0000000000000007);
					//kprintf("Address of page = %x\n", (addr & 0x000000000fffffff));
					//kprintf("Virtual Address of pageheader = %x\n", pheader->p_vaddr + i);
				}

				parent->vm_next = (struct vm_area_struct*)kmalloc((int)sizeof(struct vm_area_struct));
				parent->vm_next->vm_start = pheader->p_vaddr;
				parent->vm_next->vm_end = pheader->p_vaddr + pheader->p_filesz;
				parent->vm_next->mmstruct = (struct mm_struct*)task->mmstruct;
				//parent->vm_next->vm_flags = pheader->p_flags;
				parent->vm_next->vm_next = NULL;
				task->mmstruct->total_vm++;
				parent = (struct vm_area_struct*)parent->vm_next;

			}

			
		}
		//kprintf("Process Segment Type = %x\n", pheader->p_type);


	}
	//kprintf("The total vm's in the task are = %x\n", task->mmstruct->total_vm);
	return task;
}

void sbush()
{

}

uint64_t init_process_paging()
{
	freelistnode node = get_free_page();
	uint64_t process_pml4 = node->address;
	uint64_t kernel_pml4 = kernel_pml4_address;
	/*__asm__ volatile("movq %%cr3, %0\n"
		:"=g"(kernel_pml4)
		:
		);*/
	for(int i = 0; i < 512; i++)
	{
		*((uint64_t*)(get_virtual_address(process_pml4, 0)  + i*8)) = *((uint64_t*)(get_virtual_address(kernel_pml4, 0)  + i*8));
	}
	return process_pml4;
}

void Traverse_process_PML4(uint64_t vaddress, uint64_t paddress, uint64_t *PML4_address, uint64_t page_entry_flag)
{
	uint64_t pml4_index = get_pml4_index(vaddress);
	if((PML4_address[pml4_index] & present_bit_mask) == 0)
	{
		freelistnode pdp_page_node = get_free_page();
		PML4_address[pml4_index] = pdp_page_node->address | page_entry_flag;
	}
	uint64_t *process_pdp_address = (uint64_t*)(get_virtual_address(PML4_address[pml4_index], 0) & offset_mask);
	Traverse_process_PDP(vaddress, paddress, process_pdp_address, page_entry_flag);
}

void Traverse_process_PDP(uint64_t vaddress, uint64_t paddress, uint64_t *PDP_address, uint64_t page_entry_flag)
{
	uint64_t pdp_index = get_pdp_index(vaddress);
	if((PDP_address[pdp_index] & present_bit_mask) == 0)
	{
		freelistnode pd_page_node = get_free_page();
		PDP_address[pdp_index] = pd_page_node->address | page_entry_flag;
	}
	uint64_t *process_pd_address = (uint64_t*)(get_virtual_address(PDP_address[pdp_index], 0) & offset_mask);
	Traverse_process_PD(vaddress, paddress, process_pd_address, page_entry_flag);
}

void Traverse_process_PD(uint64_t vaddress, uint64_t paddress, uint64_t *PD_address, uint64_t page_entry_flag)
{
	uint64_t pd_index = get_pd_index(vaddress);
	if((PD_address[pd_index] & present_bit_mask) == 0)
	{
		freelistnode pt_page_node = get_free_page();
		PD_address[pd_index] = pt_page_node->address | page_entry_flag;
	}
	uint64_t *process_pt_address = (uint64_t*)(get_virtual_address(PD_address[pd_index], 0) & offset_mask);
	Traverse_process_PT(vaddress, paddress, process_pt_address, page_entry_flag);
}

void Traverse_process_PT(uint64_t vaddress, uint64_t paddress, uint64_t *PT_address, uint64_t page_entry_flag)
{
	uint64_t pt_index = get_pt_index(vaddress);
	PT_address[pt_index] = paddress | page_entry_flag;
}

void init_process_list(taskptr task)
{
	uint64_t address = kmalloc(4096*2);
	uint64_t list_capacity = (4096 * 2)/sizeof(struct process_list_node);	/**/	
	process_list = (struct process_list_node*)address;
	process_list->task = task;
	process_list->next = (struct process_list_node*)(address + sizeof(struct process_list_node));
	uint64_t temp_addr = address + sizeof(struct process_list_node);
	struct process_list_node* pnode = NULL;
	for(int i = 1; i < list_capacity; i++)
	{
		pnode = (struct process_list_node*)temp_addr;
		pnode->task = NULL;		
		temp_addr = temp_addr + sizeof(struct process_list_node);
		pnode->next = (struct process_list_node*)temp_addr;
	}	
	pnode->next = NULL;
	/*pnode = process_list;
	for(int i = 0; i < 4; i++)
	{
		pnode = pnode->next;
		kprintf("i = %d   ", i);
		//kprintf("Pid of A = %d\n", pnode->task->pid);
	}*/
	//process_list_last = (struct process_list_node*)address;
}

void Add_task_to_list(taskptr task1)
{
	/*struct process_list_node* pnode = (struct process_list_node*)process_list_last;
	kprintf("Value of rdi = %d\n", pnode->task->rdi);
	pnode->next = (struct process_list_node*)(process_list_last + sizeof(struct process_list_node));
	pnode->next->task = task;
	pnode->next->next = NULL;
	kprintf("Value of rdi = %d\n", pnode->next->task->rdi);
	process_list_last = process_list_last + sizeof(struct process_list_node);*/
	//uint64_t list_capacity = (4096 * 2)/sizeof(struct process_list_node);
	struct process_list_node* pnode = process_list;
	//kprintf("Address of process_list = %x\n", pnode);
	//kprintf("Pid of A = %d\n", pnode->task->pid);
	while(1)
	{
		if(pnode->task == NULL)
			break;
		pnode = pnode->next;
		//kprintf("%d  ", pnode->task->pid);
	}
	pnode->task = task1;
	//kprintf("%d  \n", pnode->task->pid);	
}


void Traverse_Process_list()
{
	//for(int )
	struct process_list_node* pnode = process_list;
	while(1)
	{
		if(pnode->task == NULL)
			break;
		//kprintf("\n%d  ", pnode->task->pid);
		pnode = pnode->next;
	}
}

void strcopy(char* source, char* destination)
{
	//kprintf("Value in strcpy = %s", file);
	int i = 0;
	while(source[i] != '\0')
	{
		destination[i] = source[i];
		i++;
	}
	destination[i] = '\0';
}


void ReinitialiseTask(char* file)
{
	taskptr current = me;
	char file1[100];
	strcopy(file, file1);	
	//char* file1 = file;
	//kprintf("The file to be read is %s\n", file1);
	//Clean Vma's first
	struct vm_area_struct *vma = current->mmstruct->mmmap;
	struct vm_area_struct *temp = NULL;
	for(int i = 0; i < current->mmstruct->total_vm; i++)
	{
		if(vma)
		{
			temp = vma;
			vma = vma->vm_next;
			freepage((uint64_t)temp - mapping_mask);
		}
	}
	current->mmstruct->mmmap = NULL;
	current->mmstruct->total_vm = 0;
	current->heap_address = HEAP_ADDRESS;
	//Clean page Tables now
	#if 1
	CleanPageTableEntries(current);
	readelf_1(file1, current);
	AllocateUserStackPage(current);
	updateCR3(current->cr3);
	current->rsp = (uint64_t)(&(current->kstack[511]));
	set_tss_rsp((&current->kstack[511]));	
	__asm__ volatile("movq %0, %%rsp\n"
		:
		:"g"((uint64_t)current->ustack + 511*8)
		);
	ring0_ring3((uint64_t)current);
	#endif
}

void readelf_1(char* file, taskptr task)
{
	//kprintf("Yo reading the file\n");
	//kprintf("The file to be read is %s\n", file);
	Elf64_Ehdr* eheader = (Elf64_Ehdr*)find_executable(file);
	if(eheader->e_ident[0] == 0x7f && eheader->e_ident[1] == 'E' && eheader->e_ident[2] == 'L' && eheader->e_ident[3] == 'F' && eheader->e_type == 2)
	{
		kprintf("");
	}
	//taskptr task = init_task();
	//task->mmstruct = (struct mm_struct*)kmalloc((int)sizeof(struct mm_struct));
	//kprintf("address of task mmstruct = %x\n", task->mmstruct);
	//task->rsp = (uint64_t)&task->kstack[511];

	//my_memset((void*)task->kstack, 0, 512*8);

	//task->kstack[511] = (uint64_t)eheader->e_entry;
	task->rip = (uint64_t)eheader->e_entry;
	//task->mmstruct->mmmap = NULL;
	//uint64_t process_pml4 = init_process_paging();
	//task->cr3 = process_pml4;
	//set_page_flags(process_pml4 & 0xFFFFFFFFFFFFF000, 2, mapping_mask);	
	//uint64_t process_entry_point = eheader->e_entry;
	//kprintf("process_entry_point = %x\n", process_entry_point);
	uint64_t program_header_table_address = (uint64_t)eheader + eheader->e_phoff;
	//kprintf("program_header_table_address = %x\n", program_header_table_address);
	uint64_t program_header_table_entries = eheader->e_phnum;
	//kprintf("program_header_table_entries = %x\n", program_header_table_entries);
	uint64_t program_header_table_entry_size = eheader->e_phentsize;
	struct vm_area_struct* parent = NULL;
	//task->mmstruct->mmmmap->total_vm = 0;
	for(uint64_t i = 0; i < program_header_table_entries; i++)
	{
		Elf64_Phdr* pheader = (Elf64_Phdr*)(program_header_table_address + i*program_header_table_entry_size);
		if(pheader->p_type == 1)
		{
			//Loadable section found
			//struct vm_area_struct* temp = NULL;
			//struct vm_area_struct* tempnext = task_sbush->mmstruct->mmmap;
			//kprintf("eheader address = %x, i = %d\n", eheader, i);

			if(task->mmstruct->mmmap == NULL)
			{
				uint64_t addr = memcopy((uint64_t)eheader + pheader->p_offset, (uint64_t)eheader + pheader->p_offset + pheader->p_filesz);
				//kprintf("Address of page = %x\n", addr);
				for(uint64_t i = 0; i <= pheader->p_filesz; i=i+4*1024)
				{
					Traverse_process_PML4(pheader->p_vaddr + i, (addr & 0x000000000fffffff) + i, (uint64_t*)get_virtual_address(task->cr3, 0), 0x0000000000000007);
					//kprintf("Address of page = %x\n", (addr & 0x000000000fffffff));
					//kprintf("Virtual Address of pageheader = %x\n", pheader->p_vaddr + i);
				}

				task->mmstruct->mmmap = (struct vm_area_struct*)kmalloc((int)sizeof(struct vm_area_struct));
				task->mmstruct->mmmap->vm_start = pheader->p_vaddr;
				task->mmstruct->mmmap->vm_end = pheader->p_vaddr + pheader->p_filesz;
				task->mmstruct->mmmap->mmstruct = (struct mm_struct*)task->mmstruct;
				//task->mmstruct->mmmap->vm_flags = pheader->p_flags;
				task->mmstruct->mmmap->vm_next = NULL;
				parent = (struct vm_area_struct*)task->mmstruct->mmmap;
				task->mmstruct->total_vm = 1;
			}
			else
			{
				//to be written
				uint64_t addr = memcopy((uint64_t)eheader + pheader->p_offset, (uint64_t)eheader + pheader->p_offset + pheader->p_filesz);
				//kprintf("Address of page = %x\n", addr);
				for(uint64_t i = 0; i <= pheader->p_filesz; i=i+4*1024)
				{
					Traverse_process_PML4(pheader->p_vaddr + i, (addr & 0x000000000fffffff) + i, (uint64_t*)get_virtual_address(task->cr3, 0), 0x0000000000000007);
					//kprintf("Address of page = %x\n", (addr & 0x000000000fffffff));
					//kprintf("Virtual Address of pageheader = %x\n", pheader->p_vaddr + i);
				}

				parent->vm_next = (struct vm_area_struct*)kmalloc((int)sizeof(struct vm_area_struct));
				parent->vm_next->vm_start = pheader->p_vaddr;
				parent->vm_next->vm_end = pheader->p_vaddr + pheader->p_filesz;
				parent->vm_next->mmstruct = (struct mm_struct*)task->mmstruct;
				//parent->vm_next->vm_flags = pheader->p_flags;
				parent->vm_next->vm_next = NULL;
				task->mmstruct->total_vm++;
				parent = (struct vm_area_struct*)parent->vm_next;

			}

			
		}
		//kprintf("Process Segment Type = %x\n", pheader->p_type);


	}
	//kprintf("The total vm's in the task are = %x\n", task->mmstruct->total_vm);
}


void CleanPageTableEntries(taskptr task)
{
	//kprintf("In Deep Copy\n");
	uint64_t* taskcr3 = (uint64_t*)(task->cr3 + mapping_mask);
	//uint64_t* parentcr3 = (uint64_t*)(parent->cr3 + mapping_mask);
	//kprintf("Child = %x\n", childcr3);
	//kprintf("parentcr3 = %x\n", parentcr3);
	for(int i = 0; i < 511; i++) //Since Kernel's page tables are already mapped
	{
		if(taskcr3[i]!=0)
		{
			//kprintf("here1\n");
			//kprintf("i = %d\n", i);
			//uint64_t* child_pdp = (uint64_t*)kmalloc(4096);
			uint64_t* task_pdp = (uint64_t*)((taskcr3[i] & 0xFFFFFFFFFFFFF000) + mapping_mask);
			for(int j = 0; j < 512 ; j++)
			{
				//kprintf("j = %d\n", j);
				if(task_pdp[j] != 0)
				{
					//kprintf("here2\n");
					//uint64_t* child_pd = (uint64_t*)kmalloc(4096);
					uint64_t* task_pd = (uint64_t*)((task_pdp[j] & 0xFFFFFFFFFFFFF000) + mapping_mask);
					for(int k = 0; k < 512; k++)
					{
						//kprintf("k = %d\n", k);
						if(task_pd[k] != 0)
						{
							//kprintf("here3\n");
							//uint64_t* child_pt = (uint64_t*)kmalloc(4096);
							uint64_t* task_pt = (uint64_t*)((task_pd[k] & 0xFFFFFFFFFFFFF000) + mapping_mask);
							for(int l = 0; l < 512; l++)
							{
								//kprintf("l = %d\n", l);
								if(task_pt[l] != 0)
								{
									//kprintf("here4\n");
									uint64_t page_address = task_pt[l] & 0xFFFFFFFFFFFFF000;
									uint64_t page_num = (page_address - first_page_address)/4096;
									//kprintf("Page number = %x\n", page_num);
									uint64_t node_address = free_list_start + page_num*sizeof(struct FreeListNode);
									freelistnode node = (freelistnode)(node_address + mapping_mask);
									if(node->page_usage_count <= 1)
									{
										freepage(node->address);
									}
									else
									{
										node->page_usage_count = node->page_usage_count - 1;
									}
									//node->page_usage_count++;
									//kprintf("The parent address = %x   Node- page address = %x\n", (parent_pt[l] & 0xFFFFFFFFFFFFF000), node->address);
									//child_pt[l] = (parent_pt[l] & 0xFFFFFFFFFFFFFFFD) | COW_BIT | 0x0000000000000005;
									//parent_pt[l] = (parent_pt[l] & 0xFFFFFFFFFFFFFFFD) | COW_BIT | 0x0000000000000005;
									//child_pt[l] = parent_pt[l] | 0x0000000000000007;
									task_pt[l] = 0;
								}
							}
							//child_pd[k] = (((uint64_t)child_pt - mapping_mask) | 0x0000000000000007);
							freepage(task_pd[k] & 0xFFFFFFFFFFFFF000); //free pt page
							task_pdp[k] = 0;
						}
					}
					//child_pdp[j] = (((uint64_t)child_pd - mapping_mask) | 0x0000000000000007);
					freepage(task_pdp[j] & 0xFFFFFFFFFFFFF000); //free pd page
					task_pdp[j] = 0;
				}
				
			}
			freepage(taskcr3[i] & 0xFFFFFFFFFFFFF000); // free pdp page
			taskcr3[i] = 0;			
		}
		
	}
	updateCR3(task->cr3);

}