#include<sys/pit_timer.h>
#include<sys/iofunc.h>
#include<sys/defs.h>

void init_pit()
{
	//command word for clock should be 0x34.
	//outb(0x36, 0x43); 
	outb(0x34, 0x43);

	//keeping the interuppt frequency at 18 Hz
	// 1193182/65535 = 18.2Hz
	// Set by BiOS by default.....so keeping the same frequency
	int freq_divider = 65535; 
	uint8_t flag8 = 0x00;
	outb((freq_divider | flag8), 0x40);
	outb(((freq_divider>>8) | flag8), 0x40);

	

	
}
