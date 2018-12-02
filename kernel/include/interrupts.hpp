#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#include <types.hpp>
#include <ports.hpp>
#include <gdt.hpp>


#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define MPIC1 0x20
#define MPIC2 0x21

#define SPIC1 0xA0
#define SPIC2 0xA1

#define INTTOP 256//256

//privilage levels
#define RING_0 0x0
#define RING_1 0x1
#define RING_2 0x2
#define RING_3 0x3
//
#define set_pr(x) x<<7 //set if present
#define set_dpl(x) x<<5 //set Descriptor Privilege Level
#define set_ss(x) x<<4 //Set Storage Segment - 0 for interrupt&trap gates

#define INTERRUPT_TYPE_32 0xE //32 bit interrupt
#define TRAP_TYPE_32 0xF //32 bit trap
//TODO: add call gates

#define EOI 0x20
void init_pic(byte low, byte high);

//void ocw (paddr port, byte command); //exec operational command word

void master_eoi();

void slave_eoi();


struct idtptr {
    word limit; //length in BYTES
    dword base; //pointer
} __attribute__ ((packed));

extern "C" __attribute__((fastcall)) void _lidt(idtptr* i); //AKA dword i (only address matters really)

struct idtseg{
	word offset1; //0-15 bit(low)
	word selector;
	byte zero; //zero
	byte type; //flags	
	word offset2; //48-63 bit(high)
} __attribute__ ((packed));

struct int_iden
{
//pushad = push: EAX, ECX, EDX, EBX,  ESP, EBP, ESI, EDI;
dword gs, fs, es, ds;
dword edi, esi, ebp, esp, ebx, edx, ecx, eax; //pushad
dword iden, e_code; //isr

//dword eip,cs, eflags, usp, ss; //INT

}__attribute__((packed));

extern idtptr idtp;
extern idtseg idt[INTTOP];

typedef void (*isr_func_t)(const int_iden);

extern isr_func_t int_handlers[INTTOP];

void register_interrupt_handler(word interrupt, isr_func_t fun);

void init_interrupts();

void idt_install();
bool init_idt_segs();
bool init_idt_seg(idtseg* ptr, int selector, int flags, int offset2);

extern void page_fault_handler(const int_iden); //in paging.hpp

extern "C" {
	
	void _cli();
	void _sti();
	void _nop();
	void _halt();
	
	void _c_int_handler(int_iden ii); 
//----
	addr_t get_int_handler(); 

	dword _get_isr_size(); 
	addr_t _get_isr0_addr(); 

	void int_handler(); 
/* pushad exec popad iret */

}

#endif