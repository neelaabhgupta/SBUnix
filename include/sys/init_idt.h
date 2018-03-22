#ifndef _INIT_IDT_H
#define _INIT_IDT_H
#include<sys/defs.h>


//Citation - http://wiki.osdev.org/Interrupt_Descriptor_Table
struct IDT {
   uint16_t isr_address_1; // offset bits 0..15
   uint16_t selector; // a code segment selector in GDT or LDT
   uint8_t ist;       // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
   uint8_t attribute; // type and attributes
   uint16_t isr_address_2; // offset bits 16..31
   uint32_t isr_address_3; // offset bits 32..63
   uint32_t res;     // reserved
}__attribute__((packed));

struct idt_info_register{
	uint16_t size;
	struct IDT* basepointer;
}__attribute__((packed));

void set_idt_values(int interuppt_num, uint64_t address, uint16_t selector, uint8_t attribute, uint8_t ist, uint32_t res);
void load_idt_table();

#endif
