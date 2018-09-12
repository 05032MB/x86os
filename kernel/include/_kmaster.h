#ifndef _KMASTER_H
#define _KMASTER_H

#include <stddef.h>
#include <stdint.h>
#include <types.h>
#include <interrupts.h>
#include <kmemory.h>
#include <gdt.h>

void term_clear(char bchar);
void term_init();
void term_putc(char c, uint8_t color);
void term_print(const char* str, uint8_t color);

//Assembler functions

extern "C" 
{
void halt();
void _cli();
void _sti();
void _nop();
void init_idt();
}


#endif