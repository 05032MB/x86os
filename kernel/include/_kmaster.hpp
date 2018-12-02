#ifndef _KMASTER_H
#define _KMASTER_H

#include <stddef.h>
#include <stdint.h>
#include <types.hpp>
#include <interrupts.hpp>
#include <kmemory.hpp>
#include <gdt.hpp>
#include <paging.hpp>

#include <stdlib.h>
#include <stdio.h>


  #define ARRSIZE(x)  (sizeof(x) / sizeof((x)[0]))
 
#include <input.hpp>
#include <vga.hpp>
#include <syscalls.hpp>


//Assembler functions

extern "C" 
{
void halt();
void _cli();
void _sti();
void _nop();
void init_idt();
void _lets_err();
void __attribute__((fastcall))switch_to_ring_3(void(*)());
}


#endif