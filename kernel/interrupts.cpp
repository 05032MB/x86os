#include <interrupts.hpp>

#include <ports.hpp>
#include <gdt.hpp>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern void page_fault_handler(const int_iden); //in paging.hpp

idtptr idtp;
idtseg idt[INTTOP];

isr_func_t int_handlers[INTTOP];

__ASM_IMPORT {
	
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

__used
static void dputs(int a)
{
	char tr[50];
	itoa(a, tr, 10);
	puts(tr);
}

static void ocw (paddr port, byte command)
{
	outb(port, command);
}

void init_pic(byte low, byte high)
{
	//ICW1
	byte icw = 0x11; //00010001b
	outb(MPIC1, icw);
	outb(SPIC1, icw);
	
	//ICW2
	outb(MPIC2, low);
	outb(SPIC2, high);
	
	//ICW3
	outb(MPIC2, 0x4);
	outb(SPIC2, 0x2);
	
	//ICW4
	outb(MPIC2, 0x1);
	outb(SPIC2, 0x1);
	
	outb(MPIC2, 0xFF); //disable all hardware interrupts (master&slave)
	_cli(); //aka irqs are not my job

}

void idt_install()
{
    idtp.limit = (sizeof (struct idtseg) * INTTOP) - 1;//limit = rozmiar - 1
    idtp.base = (dword) &idt;
	
    //memset(idt, 0, sizeof(idt[0])*INTTOP-1); //memset(&idt, 0, 8*256);

    _lidt((idtptr*)&idtp);
}

bool init_idt_seg(idtseg* ptr, int selector, int flags, int pointer)
{
	//add memset
	//if(num > INTTOP-1)return false;
	word low = pointer & 0xffff;
	word high = (pointer & 0xffff0000) >> 16;
	
	
	(*ptr).offset1 = low;
	(*ptr).selector = selector;
	(*ptr).zero = 0;	
	(*ptr).type = flags;
	(*ptr).offset2 = high;
	
	return true;
}

__nooptimize //otherwise some weird happens. Compile with different version and check if also occurs
bool init_idt_segs()
{
	//word goodflag = 0x8E;
	//dputs(KERNEL_CS);
	for(int i =0; i<2; i++)init_idt_seg(&idt[i], (word) KERNEL_CS,(byte) set_pr(0x1) | set_dpl(RING_0) | (INTERRUPT_TYPE_32) , _get_isr0_addr() +_get_isr_size()*i);
	for(int i =3; i<5; i++)init_idt_seg(&idt[i], (word) KERNEL_CS ,(byte) set_pr(0x1) | set_dpl(RING_0) | (TRAP_TYPE_32) , _get_isr0_addr() +_get_isr_size()*i);
	for(int i =5; i<49; i++)init_idt_seg(&idt[i], (word) KERNEL_CS ,(byte) set_pr(0x1) | set_dpl(RING_0) | (INTERRUPT_TYPE_32) , _get_isr0_addr() +_get_isr_size()*i);
	init_idt_seg(&idt[100], (word) 0x8 ,(byte) set_pr(0x1) | set_dpl(RING_0) | (INTERRUPT_TYPE_32) , _get_isr0_addr() +_get_isr_size()*100);
	
	init_idt_seg(&idt[80], (word) KERNEL_CS ,(byte) set_pr(0x1) | set_dpl(RING_3) | (INTERRUPT_TYPE_32) , _get_isr0_addr() + _get_isr_size()*80); //reserved for syscalls
	init_idt_seg(&idt[0x65], (word) KERNEL_CS ,(byte) set_pr(0x1) | set_dpl(RING_0) | (INTERRUPT_TYPE_32) , _get_isr0_addr() + _get_isr_size()*0x65); //system test interrupt

	//init_idt_seg(&idt[14], (word)KERNEL_CS, set_pr(1) | set_dpl(RING_0) | INTERRUPT_TYPE_32, _get_isr0_addr() + _get_isr_size()*14);//pfault

	ocw(MPIC2, 0xFD); //odmaskowuje irq2
	
	_sti(); //zaczynamy obsługiwać irq
	
	return true;
}

void master_eoi()
{
	ocw(MPIC1, EOI);
}
void slave_eoi()
{
	ocw(SPIC1, EOI);
	ocw(MPIC1, EOI);
}

void register_interrupt_handler(word interrupt, isr_func_t fun)
{
	int_handlers[interrupt] = fun;
}

__nooptimize
void _c_int_handler(const int_iden ii) 
 {
	
	const char* msg[] = {
	"#DE", "#DB", "NMI", "#BP", "#OF", "#BR",
	"#UD", "#NM", "#DF", "CoprocessorSegmentOverrun", "#TS", "#NP", "#SS",
	"#GP", "#PF", "IR1", "#MF", "#AC", "#MC", "#XM", "#VE",
	"IR2", "IR3", "IR4", "IR5", "IR6", "IR7", "IR8", "IR9", "IR10",
	"#SX", "IR11", "irq1", "irq2", "irq3", "irq4", "irq5", "irq6", "irq7", "irq8", "irq9", "irq10",
	"irq11", "irq12", "irq13", "irq15", "irq16", "irq17", "irq18",
	};
	msg[80]="SYSCALL";

	#ifdef __k_debug
	//dputs(ii.iden);
	//if(ii.iden <= 48 || ii.iden == 80)puts(msg[ii.iden]);
	int z =0;
	//if(ii.iden != 33){/*halt();*/while(z++ < 100000000)_nop();}
	#endif
	
	//dputs((int)int_handlers[ii.iden]);
	if(int_handlers[ii.iden])int_handlers[ii.iden](ii);
	
	if(ii.iden >= 0x20 && ii.iden < 0x28)master_eoi();
	else if(ii.iden >= 0x28 && ii.iden < 0x2f /* 47 */){
		slave_eoi();
		master_eoi();
	}
	
 }

 void init_interrupts()
 {
 
	memset(idt, 0, sizeof(idt[0])*INTTOP);
 
	init_pic(0x20, 0x28); //32, 40
	init_idt_segs();	
	idt_install();
	
	register_interrupt_handler(14, page_fault_handler);
	
 }