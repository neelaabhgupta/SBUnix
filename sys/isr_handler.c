#include<sys/kprintf.h>
#include<sys/isr_handler.h>
//#include<sys/defs.h>
#include<sys/iofunc.h>
#include<stdarg.h>
#include<sys/terminal.h>
#include<sys/process_scheduling.h>
#include<sys/init_pd.h>

int flag = 0;
int sleep_flag = 0;
int timer_capacity = 0;
int is_sleep = 0;
int sleepf = 0;
//int keyboard_flag = 1;
int shift_flag = 0;
int caps_flag = 0;
int control_flag = 0;

char make_code_map[256] = {0};

int s_counter = 1;
int m_counter = 0;
int h_counter = 0;
void tprintf(char *fmt, ...);

void init_keyboard_map()
{
	//Alphabets
	make_code_map[0x1E] = (char)97;
	make_code_map[0x30] = (char)98;
	make_code_map[0x2E] = (char)99;
	make_code_map[0x20] = (char)100;
	make_code_map[0x12] = (char)101;
	make_code_map[0x21] = (char)102;
	make_code_map[0x22] = (char)103;
	make_code_map[0x23] = (char)104;
	make_code_map[0x17] = (char)105;
	make_code_map[0x24] = (char)106;
	make_code_map[0x25] = (char)107;
	make_code_map[0x26] = (char)108;
	make_code_map[0x32] = (char)109;
	make_code_map[0x31] = (char)110;
	make_code_map[0x18] = (char)111;
	make_code_map[0x19] = (char)112;
	make_code_map[0x10] = (char)113;
	make_code_map[0x13] = (char)114;
	make_code_map[0x1F] = (char)115;
	make_code_map[0x14] = (char)116;
	make_code_map[0x16] = (char)117;
	make_code_map[0x2F] = (char)118;
	make_code_map[0x11] = (char)119;
	make_code_map[0x2D] = (char)120;
	make_code_map[0x15] = (char)121;
	make_code_map[0x2C] = (char)122;

	//Numbers
	make_code_map[0x0B] = (char)48;
	make_code_map[0x02] = (char)49;
	make_code_map[0x03] = (char)50;
	make_code_map[0x04] = (char)51;
	make_code_map[0x05] = (char)52;
	make_code_map[0x06] = (char)53;
	make_code_map[0x07] = (char)54;
	make_code_map[0x08] = (char)55;
	make_code_map[0x09] = (char)56;
	make_code_map[0x0A] = (char)57;

	//Special Keys

	////Enter Key
	make_code_map[0x1C] = (char)10;

	//Space Key
	make_code_map[0x39] = (char)32;

	//left shift
	make_code_map[0x2A] = 'S';
	make_code_map[0x36] = 'S';

	//control
	make_code_map[0x1D] = 'C';
	make_code_map[0xE0] = 'C';

	//caps lock key
	make_code_map[0x3A] = 'L';
	make_code_map[0x0E] = 'B';
	make_code_map[0x0C] = '-';
	make_code_map[0x2B] = '\\';
	make_code_map[0x35] = '/';
	make_code_map[0x34] = '.';
	make_code_map[0x0D] = '=';
	make_code_map[0x1A] = '[';
	make_code_map[0x1B] = ']';
	make_code_map[0x27] = ';';
	make_code_map[0x28] = '\'';
	make_code_map[0x33] = ',';


} 

void isr_handler_32()
{
	//kprintf("Timer Start");
	if(is_sleep == 1)
	{
		sleep_flag++;
		if(sleep_flag == timer_capacity)
		{
			is_sleep = 0;
		}
	}
	if(flag == 18)
	{	
		if(s_counter < 60)
		{	
			if(s_counter < 10)
			{
				tprintf("00:00:0%d", s_counter);
			}
			else
			{
				tprintf("00:00:%d", s_counter);
			}
			
		}
		else if(s_counter >= 60 && s_counter < 3600)
		{
			m_counter = s_counter/60;
			if(s_counter - m_counter*60 == 0)
			{
				tprintf("00:0%d:00", m_counter);
			}
			if(m_counter < 10)
			{
				if(s_counter-m_counter*60 < 10)
				{
					tprintf("00:0%d:0%d", m_counter, ((s_counter - m_counter*60)));
				}
				else
				{
					tprintf("00:0%d:%d", m_counter, (s_counter - m_counter*60));
				}
			}
			else
			{
				if(s_counter-60 < 10)
				{
					tprintf("00:%d:0%d", m_counter, (s_counter - m_counter*60));
				}
				else 
				{
					tprintf("00:%d:%d", m_counter, (s_counter - m_counter*60));	
				}
				
			}
		}
		else
		{
			m_counter = s_counter/60;
			h_counter = m_counter/60;
			if(h_counter < 10)
			{
				if(s_counter - m_counter*60 < 10)
				{
					if(s_counter - h_counter*3600 < 10)
					{
						tprintf("0%d:0%d:0%d",h_counter ,m_counter, ((s_counter - m_counter*60)));
					}
					else
					{
						tprintf("0%d:0%d:%d",h_counter ,m_counter, ((s_counter - m_counter*60)));
					}
				}
				else
				{
					if(s_counter - h_counter*3600 < 10)
					{
						tprintf("0%d:%d:0%d",h_counter ,m_counter, ((s_counter - m_counter*60)));
					}
					else
					{
						tprintf("0%d:%d:%d",h_counter ,m_counter, ((s_counter - m_counter*60)));
					}
				}
			}
			else
			{
				if(s_counter - m_counter*60 < 10)
				{
					if(s_counter - h_counter*3600 < 10)
					{
						tprintf("%d:0%d:0%d",h_counter ,m_counter, ((s_counter - m_counter*60)));
					}
					else
					{
						tprintf("%d:0%d:%d",h_counter ,m_counter, ((s_counter - m_counter*60)));
					}
				}
				else
				{
					if(s_counter - h_counter*3600 < 10)
					{
						tprintf("%d:%d:0%d",h_counter ,m_counter, ((s_counter - m_counter*60)));
					}
					else
					{
						tprintf("%d:%d:%d",h_counter ,m_counter, ((s_counter - m_counter*60)));
					}
				}
			}
		}
		flag = 0;
		s_counter++;
	}
	else 
	{
		flag++;
		//sleep_flag++;
	}
	//EOI
	outb(0x20,0x20);
	outb(0x20,0xA0);
}


void tprintf(char *fmt, ...)
{
    va_list vl;
    //char *p;
    va_start(vl, fmt);
    int x = 3984;
    //int counter =0;
    char *current_vp = (char*)(0xb8000 + 0xFFFFFFFF80000000);
    current_vp = current_vp + x;
    for (int l = 0; fmt[l] != '\0'; l++) 
    {
        if (fmt[l] != '%') 
        {
            *current_vp++ = fmt[l];
            *current_vp++ = 0x1F;
            continue;
        }
        switch (fmt[++l])
        {
            case 'd':
            {
                int i = 0;
                char temp[1024] = {};
                int mod = 0;
                int val = va_arg(vl, int);
                while(val)
                {
                    mod = val%10;       
                    temp[i] = mod+'0';
                    val = val/10;
                    i++;
                }
                i--;
                while(i >= 0)
                {
                    *current_vp++ = temp[i--];
                    *current_vp++ = 0x0F;
                }
            }
            break;
        }
    } 
    va_end(vl);
    //vp_counter++;
}  

/*void isr_handler_32()
{
	//kprintf("Timer Start");
	if(flag == 18)
	{	
		//kprintf("one ");
		flag = 0;
	}
	else flag++;
	outb(0x20,0x20);
	outb(0x20,0xA0);
}*/


//Ciation - https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
int check_make_code(uint8_t keyboard_code)
{
	//check for the highest order bit. if 1 break code otherwise make code.
	if(keyboard_code & 0x80)
	{
		return 0;
	}
	else return 1;
}

//Citation - http://www.brokenthorn.com/Resources/OSDev19.html
void isr_handler_33()
{	
	//shift_flag=3;
	//kprintf("Keyboard Interuppt");
	
	uint8_t keyboard_code = inb(0x60);
	if(check_make_code(keyboard_code))
	{
		//kprintf("%c", make_code_map[keyboard_code]);
		//char *glyph_video_ptr = (char *)(0xb8000 + 0xFFFFFFFF80000000 + 3980);
		switch(make_code_map[keyboard_code])
		{
			case 'S':
			{	
				//kprintf("Shift pressed");
				//Shift Key Pressed.
				shift_flag = 1;
				//kprintf("\nflag=%d",shift_flag);
			}
			break;
			case 'C':
			{
				/**glyph_video_ptr++ = '^';
				*glyph_video_ptr++ = 0x1F;
				*glyph_video_ptr++ = 'C';
				*glyph_video_ptr = 0x1F;*/
				control_flag = 1;
			}
			break;
			case 'E':
			{
				/**glyph_video_ptr++ = '^';
				*glyph_video_ptr++ = 0x1F;
				*glyph_video_ptr++ = 'M';
				*glyph_video_ptr = 0x1F;*/
			}
			break;
			case (char)10:
			{
				buff_write('\n');
			}
			break;
			case 'B':
			{
				backspace();
			}
			break;
			case 'L':
			{
				//caps lock key
				caps_flag = 1;
			}
			break;
			default:
			{
				if(shift_flag)
				{
					if(make_code_map[keyboard_code] >= (char)97 && make_code_map[keyboard_code] <= (char)122)
					{
						// *glyph_video_ptr++ =  make_code_map[keyboard_code] - (char)32;
						// *glyph_video_ptr++ = 0x1F;
						// *glyph_video_ptr++ = ' ';
						// *glyph_video_ptr = 0x1F;
						buff_write(make_code_map[keyboard_code] - (char)32);
					}
					else if(make_code_map[keyboard_code] == '\'')
					{
						buff_write('"');
					}
					else
					{
						// *glyph_video_ptr++ =  make_code_map[keyboard_code];
						// *glyph_video_ptr++ = 0x1F;
						// *glyph_video_ptr++ = ' ';
						// *glyph_video_ptr = 0x1F;
						buff_write(make_code_map[keyboard_code]);
					}
				}
				else if(control_flag)
				{
					// *glyph_video_ptr++ =  '^';
					// *glyph_video_ptr++ = 0x1F;
					// *glyph_video_ptr++ = make_code_map[keyboard_code] - (char)32;
					// *glyph_video_ptr = 0x1F;
					buff_write('^');
					buff_write(make_code_map[keyboard_code] - (char)32);
				}
				else if(caps_flag)
				{
					if(make_code_map[keyboard_code] >= (char)97 && make_code_map[keyboard_code] <= (char)122)
					{
						// *glyph_video_ptr++ =   make_code_map[keyboard_code] - (char)32;
						// *glyph_video_ptr++ = 0x1F;
						buff_write(make_code_map[keyboard_code] - (char)32);
					}
					else
					{
						// *glyph_video_ptr++ =   make_code_map[keyboard_code];
						// *glyph_video_ptr++ = 0x1F;
						buff_write(make_code_map[keyboard_code]);
					}
					// *glyph_video_ptr++ = ' ';
					// *glyph_video_ptr = 0x1F;
					//buff_write(' ');
				}
				else
				{
					// *glyph_video_ptr++ =  make_code_map[keyboard_code];
					// *glyph_video_ptr++ = 0x1F;
					// *glyph_video_ptr++ = ' ';
					// *glyph_video_ptr = 0x1F;
					buff_write(make_code_map[keyboard_code]);
					//buff_write(' ');
				}				
			}
			break;
		}
		
		//keyboard_flag = 0;
	}
	else
	{
		//for handling break codes
		//keyboard_flag = 1;
		if(keyboard_code == 0xAA || keyboard_code == 0xB6)
		{
			shift_flag = 0;
		}
		else if(keyboard_code == 0x9D || keyboard_code == 0xE0)
		{
			control_flag = 0;
		}
		else if(keyboard_code == 0xBA)
		{
			caps_flag = 0;
		}
	}
	//kprintf("Current keyboard_code = %d", keyboard_code);
	//EOI
	outb(0x20,0x20);
	outb(0x20,0xA0);

}


void isr_handler_14()
{
	//kprintf("Page Fault Occured!!!!!!\n");

	uint64_t cr3_address, cr2_address;
	__asm__ volatile("movq %%cr2, %0\n"
		:"=g"(cr2_address)
		:
		);
	//kprintf("CR2 Address in isr 14 = %x\n", cr2_address);
	__asm__ volatile("movq %%cr3, %0\n"
		:"=g"(cr3_address)
		:
		);
	//kprintf("CR3 Address in isr 14 = %x\n", cr3_address);
	//kprintf("Task CR3 Address in isr 14 = %x\n", me->cr3);
	//kprintf("Value of pid = %x\n", me->pid);
	//kprintf("The physical address not present is = %x\n", get_physical_address(cr2_address, (uint64_t*)(cr3_address + mapping_mask)));
	//kprintf("CR3 Address in isr 14 = %x\n", cr3_address);
	//kprintf("me CR3 Address in isr 14 = %x\n", me->cr3);
	//kprintf("difference in address = %x\n", (STACK_ADDRESS - cr2_address));
	//kprintf("differnece in pages = %d\n", (STACK_ADDRESS - cr2_address)/4096);
	//kprintf("Address to be handled = %x\n", (cr2_address & 0xfffffffffffff000));
	if(get_physical_address(cr2_address, (uint64_t*)(cr3_address + mapping_mask)) & 0x0000000000000400)
	{
		uint64_t page_address = get_physical_address(cr2_address, (uint64_t*)(cr3_address + mapping_mask)) & 0xfffffffffffff000;
		//kprintf("page_address = %x\n", page_address);
		//uint64_t page_address = parent_pt[l] & 0xFFFFFFFFFFFFF000;
		uint64_t page_num = (page_address - first_page_address)/4096;
		//kprintf("Page number = %x\n", page_num);
		uint64_t node_address = free_list_start + page_num*sizeof(struct FreeListNode);
		freelistnode node = (freelistnode)(node_address + mapping_mask);
		//kprintf("user_page_count = %d\n", node->page_usage_count);
		if(node->page_usage_count > 1)
		{
			uint64_t new_page_address = memcopy(page_address + mapping_mask, page_address + mapping_mask + 4095);
			Traverse_process_PML4(cr2_address & 0xfffffffffffff000, new_page_address - mapping_mask, (uint64_t*)(cr3_address + mapping_mask), 0x0000000000000007);
			node->page_usage_count = node->page_usage_count - 1;
			updateCR3(cr3_address);
		}
		else
		{
			Traverse_process_PML4(cr2_address & 0xfffffffffffff000, page_address, (uint64_t*)(cr3_address + mapping_mask), 0x0000000000000007);
			//set_page_flags()
			updateCR3(cr3_address);
		}
	}
	else if((STACK_ADDRESS - cr2_address)/4096 <= STACK_LIMIT)
	{
		//kprintf("in if\n");
		taskptr task = me;
		uint64_t addr = kmalloc(4096);
		Traverse_process_PML4(cr2_address & 0xfffffffffffff000, addr - mapping_mask, (uint64_t*)get_virtual_address(task->cr3, 0), 0x0000000000000007);
		updateCR3(task->cr3);
		struct vm_area_struct* temp = task->mmstruct->mmmap;
		for(int i = 0; i < task->mmstruct->total_vm; i++)
		{
			if(temp->vm_start == (STACK_ADDRESS + 4096))
			{
				//kprintf("in if2\n");
				temp->vm_end = temp->vm_end - 4096;
				break;
			}
			temp = temp->vm_next;
		}
	}
	else
	{
		kprintf("Page Fault Occured!\n");
		while(1);
	}

	/*int *a = (int*)0x6FFFFFF;
 	*a = 12;*/	
	outb(0x20,0x20);
	outb(0x20,0xA0);
}
void isr_handler_8()
{
	kprintf("Double Fault Occured!\n");
	uint64_t cr3_address, cr2_address;
	__asm__ volatile("movq %%cr2, %0\n"
		:"=g"(cr2_address)
		:
		);
	//kprintf("CR2 Address in isr 14 = %x\n", cr2_address);
	__asm__ volatile("movq %%cr3, %0\n"
		:"=g"(cr3_address)
		:
		);
	//kprintf("CR3 Address in isr 14 = %x\n", cr3_address);
	//kprintf("Task CR3 Address in isr 14 = %x\n", me->cr3);
	//kprintf("Value of pid = %x\n", me->pid);
	while(1);
	outb(0x20,0x20);
	outb(0x20,0xA0);
}


void isr_handler_128()
{
	//kprintf("in isr _128 = %x\n", 0);
	//return 1;
	//syscall_regs registers;
	/*uint64_t syscall_num = 100;
	//uint64_t reg_addr = 0;
	uint64_t string;
	
	__asm__ volatile("mov %%r13, %0;\n"
		:"=r"(string)
		);
	__asm__ volatile("mov %%r12, %0;\n"
		:"=r"(syscall_num)
		);
	
	//kprintf("in isr 128 val of syscall_num = %x\n", **syscall_num);
	//uint64_t syscall_num1 = *syscall_num;
	kprintf("in isr 128 val of syscall_num1 = %d\n", syscall_num);
	//kprintf("in isr 128 val of reg_addr = %x\n", reg_addr);
	//registers = (syscall_regs)reg_addr;
	//kprintf("in isr 128 val of registers rdi= %s\n", registers->rdi);
	return syscall_param1(syscall_num, string);
	//while(1);*/
	syscall();
	outb(0x20,0x20);
	outb(0x20,0xA0);
	return;
}

void set_sleep_flag()
{
	is_sleep = 1;
	sleep_flag = 0;
}

int get_sleep_flag()
{
	return sleep_flag;
}

void unset_sleep_flag()
{
	is_sleep = 0;
	sleep_flag = 0;
}