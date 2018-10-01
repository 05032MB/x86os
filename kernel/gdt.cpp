#include <gdt.hpp>

gdtptr gdtp;
gdtentry gdt[GDTMAX];

#ifdef __k_debug
#include <stdio.h>
#include <stdlib.h>
__attribute__((used))
static void dputs(int a)
{
	char tr[50];
	itoa(a, tr, 10);
	puts(tr);
}
#endif

void gdt_install(unsigned num)//num - number of entries
{
	gdtp.base = (dword)&gdt;
	
	gdtp.limit =( sizeof(struct gdtentry) * num )-1;
	
	_lgdt((gdtptr*)&gdtp);

}
/*
aflags - first 8 bits are access, rst flags
*/
void init_gdt_entry(gdtentry* ptr, dword base, dword limit, dword aflags) 
{
	ptr->base1 = base & 0xFFFF;
	ptr->base2 = (base >> 16) & 0xFF;
	ptr->base3 = (base >> 24) & 0xFF;
	
	ptr->limit1 = limit & 0xFFFF;
	
	byte limit2 = (limit >>16) & 0xF;
	
	ptr->access= aflags & 0xFF; 
	ptr->limflags = (aflags >> 8)| limit2; 

}
sel_t get_segment_selector_GDT(short num, short priv)
{

	return (num << 3) | priv;

}

sel_t get_code_selector()
{
	return get_segment_selector_GDT(1,0);
}

void init_gdt_entries()
{
	init_gdt_entry(&gdt[0], 0, 0, 0);
	init_gdt_entry(&gdt[1], 0, 0xFFFFFFFF, GDT_CODE_PL0); //0x8
	init_gdt_entry(&gdt[2], 0, 0xFFFFFFFF, GDT_DATA_PL0); //0x10
	init_gdt_entry(&gdt[3], 0, 0xFFFFFFFF, GDT_CODE_PL3); //0x1B
	init_gdt_entry(&gdt[4], 0, 0xFFFFFFFF, GDT_DATA_PL0); //0x23
	
}

void init_gdt()
{

	init_gdt_entries();
	gdt_install(GDTMAX);
	_on_gdt_change(get_segment_selector_GDT(2,0), get_segment_selector_GDT(1,0));//data(ecx),ccode(edx)
	//dputs(KERNEL_CS);
}