#include<sys/pic_interuppt.h>
#include<sys/kprintf.h>
#include<sys/iofunc.h>

//Citation - http://wiki.osdev.org/8259_PIC
//Citation - https://web.archive.org/web/20170702212722/http://www.brokenthorn.com/Resources/OSDevPic.html



void init_pic()
{
	unsigned mask1 = inb(0x21);
	unsigned mask2 = inb(0xA1);	

	outb(0x11, 0x20); //Send the initialisation icw(icw1) to master pic command register
	outb(0x11, 0xA0); //Send the initialisation icw(icw1) to slave pic command register

	outb(0x20, 0x21); //Send icw2 to master pic data fregister to map irqs from number 32
	outb(0x28, 0xA1); ////Send icw2 to master pic data fregister to map irqs from number 40

	outb(0x04, 0x21); //Send icw3 to master pic to let it know that it is connected to slave pic by irq2
	outb(0x02, 0xA1); ////Send icw3 to slave pic to let it know that it is connected to master pic by irq2

	outb(0x01, 0x21); //send the icw4 to master pic to enable it for 8086
	outb(0x01, 0x21); //send the icw4 to slave pic to enable it for 8086

	outb(0x02, 0x21); //send the icw4 to master pic to enable eoi
	outb(0x02, 0x21); //send the icw4 to slave pic to enable eoi

	outb(mask1, 0x21);
	outb(mask2, 0xA1);
}



