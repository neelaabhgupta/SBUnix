#ifndef __KPRINTF_H
#define __KPRINTF_H

extern int vp_counter;

void kprintf(char *fmt, ...);
void endpage();
void kprintf_bsp();
void clrscr();
#endif
