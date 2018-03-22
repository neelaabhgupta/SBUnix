#include<sys/init_idt.h>
#include<sys/isr_handler.h>
#include<sys/kprintf.h>
struct IDT idt_table[256];
struct idt_info_register idt_info_reg;
void init_lidt(struct idt_info_register* reg);

void set_idt_values(int interuppt_num, uint64_t address, uint16_t selector, uint8_t attribute, uint8_t ist, uint32_t res)
{
	uint16_t flag16 = 0x0000; 
	uint32_t flag32 = 0x00000000;
	idt_table[interuppt_num].selector = selector;
	idt_table[interuppt_num].attribute = attribute;
	idt_table[interuppt_num].res = res;
	idt_table[interuppt_num].isr_address_1 = (uint16_t)address | flag16;
	idt_table[interuppt_num].isr_address_2 = (uint16_t)((address>>16) | flag16);
	idt_table[interuppt_num].isr_address_3 = (uint32_t)((address>>32) | flag32);
	idt_table[interuppt_num].ist = ist;
}

void load_idt_table()
{
	//struct IDT idt_table[256] = {0};

	//struct idt_info_register idt_info_reg;

	idt_info_reg.size = sizeof(struct IDT)*256 - 1;
	idt_info_reg.basepointer = idt_table;

	//interuppt number 32 for timer interuppt
	set_idt_values(32, (uint64_t)isr_num32, (uint16_t)8, (uint8_t)0xEE, (uint8_t)0, (uint32_t)0);
	//interuppt number 33 for keyboard interuppt
	set_idt_values(33, (uint64_t)isr_num33, (uint16_t)8, (uint8_t)0xEE, (uint8_t)0, (uint32_t)0);
	set_idt_values(14, (uint64_t)isr_num14, (uint16_t)8, (uint8_t)0xEE, (uint8_t)0, (uint32_t)0); 
	set_idt_values(8, (uint64_t)isr_num8, (uint16_t)8, (uint8_t)0xEE, (uint8_t)0, (uint32_t)0);
	set_idt_values(128, (uint64_t)isr_num128, (uint16_t)8, (uint8_t)0xEE, (uint8_t)0, (uint32_t)0);  
	init_keyboard_map();
	/*kprintf("address of the function is : %p\n",isr_num32);
	struct IDT idt32 = idt_table[32];
	kprintf("address of the function is : %p%p%p\n",idt32.isr_address_3,idt32.isr_address_2,idt32.isr_address_1);*/
	init_lidt(&idt_info_reg);
}