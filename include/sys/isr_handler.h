#ifndef _ISR_HANDLER_H
#define _ISR_DEFINE_H
#include<sys/defs.h>
#include<sys/sys_calls.h>

int sleep_flag;
int is_sleep;
int timer_capacity;
int sleepf;
extern void isr_num32();
extern void isr_num33();
extern void isr_num14();
extern void isr_num8();
extern void isr_num128();

void init_keyboard_map();
int check_make_code(uint8_t keyboard_code);
void isr_handler_32();
void isr_handler_33();
void isr_handler_14();
void isr_handler_8();

void isr_handler_128();
void set_sleep_flag();
int get_sleep_flag();
void unset_sleep_flag();


#endif