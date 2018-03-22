#include<sys/terminal.h>

int BUFF_READY = 0;

void init_terminal()
{
	buffer_index = 0;
}

void buff_write(char c)
{
	if(c == '\n')
	{
		//kprintf("%s", buffer);
		//GetBuffer();
		BUFF_READY = 1;
		kprintf("\n");
		//send to the user process
	}
	else
	{
		buffer[buffer_index++] = c;
		kprintf("%c", c);
	}
}

void backspace()
{
	buffer_index--;
	//kprintf("here in backspace");
	kprintf_bsp();

}

void clear_buffer()
{
	for(int i = 0; i < TERMINAL_BUFFER_CAPACITY; i++)
	{
		buffer[i] = '\0';
		
	}
	buffer_index = 0;
	BUFF_READY = 0;
}