#ifndef _GDT_H
#define _GDT_H

#include <types.hpp>


//access bytes

#define SEG_AC(x)  ((x) << 0x00) // Accessed
#define SEG_RW(x)  ((x) << 0x01) // Read/Write
#define SEG_DC(x)  ((x) << 0x02) // Direction/Conforming Bit
#define SEG_EX(x)  ((x) << 0x03) // Executable bit

//
#define SEG_DESCTYPE(x)  ((x) << 0x04) // Descriptor type (0 for system, 1 for code/data)
#define SEG_PRIV(x)     (((x) &  0x03) << 0x05)   // Set privilege level (0 - 3) (uses 2 bits)
#define SEG_PRES(x)      ((x) << 0x07) // Present

//flags (from least significant bit) 
//shift left for additional 4 empty bits. makes life easier later
#define SEG_SAVL(x)      ((x) << 0x0C) // Available for system use
#define SEG_LONG(x)      ((x) << 0x0D) // Long mode
#define SEG_SIZE(x)      ((x) << 0x0E) // Size (0 for 16-bit, 1 for 32)
#define SEG_GRAN(x)      ((x) << 0x0F) // Granularity (0 for 1B - 1MB, 1 for 4KB - 4GB)


 
#define SEG_DATA_RD        0x00 // Read-Only
#define SEG_DATA_RDA       0x01 // Read-Only, accessed
#define SEG_DATA_RDWR      0x02 // Read/Write
#define SEG_DATA_RDWRA     0x03 // Read/Write, accessed
#define SEG_DATA_RDEXPD    0x04 // Read-Only, expand-down
#define SEG_DATA_RDEXPDA   0x05 // Read-Only, expand-down, accessed
#define SEG_DATA_RDWREXPD  0x06 // Read/Write, expand-down
#define SEG_DATA_RDWREXPDA 0x07 // Read/Write, expand-down, accessed
#define SEG_CODE_EX        0x08 // Execute-Only
#define SEG_CODE_EXA       0x09 // Execute-Only, accessed
#define SEG_CODE_EXRD      0x0A // Execute/Read
#define SEG_CODE_EXRDA     0x0B // Execute/Read, accessed
#define SEG_CODE_EXC       0x0C // Execute-Only, conforming
#define SEG_CODE_EXCA      0x0D // Execute-Only, conforming, accessed
#define SEG_CODE_EXRDC     0x0E // Execute/Read, conforming
#define SEG_CODE_EXRDCA    0x0F // Execute/Read, conforming, accessed
 
#define GDT_CODE_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_CODE_EXRD
 
#define GDT_DATA_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_DATA_RDWR
 
#define GDT_CODE_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(3)     | SEG_CODE_EXRD
 
#define GDT_DATA_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(3)     | SEG_DATA_RDWR

//thanks to wiki.osdev.org/GDT for providing this nice list, so I dont have to code it for yearz
#define GDTMAX 6

typedef word sel_t; //segment selector type

void init_gdt();

void init_gdt_entries();
//////////
struct gdtptr{
	word limit;
	dword base;
}__packed;


struct gdtentry{
	word limit1;
	word base1; //low(0-15)
	byte base2; //high(16-23)
	byte access;
	byte limflags; //limit 16-19 & flags 0-3
	byte base3; //highest(24-31)
}__packed;

void gdt_install();

void init_gdt_entry(gdtentry* ptr,dword base, dword limit, dword aflags);


sel_t get_segment_selector_GDT(short num, short priv);

//extern gdtptr gdtp;
//extern gdtentry gdt[GDTMAX];

extern word KERNEL_CS;
extern word KERNEL_DS;
extern word USER_DS;
extern word USER_CS;

//tss struct x86
//nilX represents reserved bits which are set to 0
struct tss_entry
{ 
   word prev_task_link, nil1;
   dword esp0;      
   word ss0, nil2;        
   dword esp1;      
   word ss1, nil3;
   dword esp2;
   word ss2, nil4;
   dword cr3;
   dword eip;
   dword eflags;
   dword eax;
   dword ecx;
   dword edx;
   dword ebx;
   dword esp;
   dword ebp;
   dword esi;
   dword edi;
   word es , nil5;         
   word cs, nil6;        
   word ss, nil7;         
   word ds, nil8;         
   word fs, nil9;         
   word gs, nil10;         
   word ldtr, nil11;        
   word T, iomba_offset; //T is debug trap(in reality 1 bit), iomba is I/O map base address field
} __packed;


__ASM_IMPORT {
__fastcall void _lgdt (gdtptr *i); 
__fastcall void _on_gdt_change (sel_t data, sel_t code); //data&code selectors respectively
__fastcall void _set_userspace_selectors (sel_t data, sel_t code); //data&code selectors respectively
__fastcall void _gdt_flush(sel_t gdt_sel);

__fastcall void _tss_flush(sel_t tss_sel);
addr_t _get_esp(void);

}

#endif