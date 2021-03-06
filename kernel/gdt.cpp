#include <gdt.hpp>

#include <string.h>	
#include <types.hpp>
#include <logger.hpp>

#ifdef HI_MEM_GDT
gdtptr gdtp;
gdtentry gdt[GDTMAX];
#else
gdtptr *gdtp = reinterpret_cast<gdtptr*>(0x600);
gdtentry *gdt = reinterpret_cast<gdtentry*>(0x7E00); //1542
#endif
tss_entry tss_main;

__ASM_IMPORT void * auxillary_stack_top;

void gdt_install(unsigned num)//num - number of entries
{
#ifdef HI_MEM_GDT
	gdtp.base = reinterpret_cast<dword>(&gdt);
	gdtp.limit =( sizeof(struct gdtentry) * num )-1; //<--!
	_lgdt((gdtptr*)&gdtp);
#else
	gdtp->base = reinterpret_cast<dword>(gdt);
	gdtp->limit =( sizeof(struct gdtentry) * num )-1; //<--!
	_lgdt(gdtp);
#endif

}
/*
aflags - first 8 bits are access, rst flags
*/
void init_gdt_entry(gdtentry* ptr, dword base, dword limit, dword aflags) 
{
	//add memset
	ptr->base1 = base & 0xFFFF;
	ptr->base2 = (base >> 16) & 0xFF;
	ptr->base3 = (base >> 24) & 0xFF;
	
	ptr->limit1 = limit & 0xFFFF;
	
	byte limit2 = (limit >>16) & 0xF;
	
	ptr->access= aflags & 0xFF; 
	ptr->limflags = (aflags >> 8)| limit2; 

}
/*
translates gdt entry index to segment selector
*/
sel_t get_segment_selector_GDT(short num, short privl)
{

	return (num << 3) | privl;

}

/*sel_t get_code_selector() //gets kernel code selector
{
	return get_segment_selector_GDT(1,0);
}*/

void init_gdt_entries()
{
	init_gdt_entry(&gdt[0], 0, 0, 0);
	init_gdt_entry(&gdt[1], 0, 0xFFFFFFFF, GDT_CODE_PL0); //0x8
	init_gdt_entry(&gdt[2], 0, 0xFFFFFFFF, GDT_DATA_PL0); //0x10
	init_gdt_entry(&gdt[3], 0, 0xFFFFFFFF, GDT_CODE_PL3); //0x1B
	init_gdt_entry(&gdt[4], 0, 0xFFFFFFFF, GDT_DATA_PL3); //0x23
	init_gdt_entry(&gdt[5], to_addr_t(&tss_main),to_addr_t(&tss_main)+ sizeof(tss_main), (SEG_GRAN(0) | SEG_EX(1) |  SEG_PRIV(3) | SEG_PRES(1) | SEG_AC(1) | SEG_DESCTYPE(0))); //mandatory tss entry
	
	tss_main.esp0 = reinterpret_cast<dword>(&auxillary_stack_top); //lepiej //_get_esp(); //fix it !!!! !!!!

	term_log("Auxillary stack is at: ",tss_main.esp0, LOG_MINOR);

	tss_main.ss0 = get_segment_selector_GDT(2,0);
	
	//tss_main.esp2 = 0xA00000+200;

	tss_main.cs = get_segment_selector_GDT(1,3);
	tss_main.ss = tss_main.ds = tss_main.es = tss_main.fs = tss_main.gs = get_segment_selector_GDT(2,3);
}

void init_gdt()
{
	memset(gdt, 0, sizeof(gdt[0])*GDTMAX);
	memset(&tss_main, 0, sizeof(tss_entry));

	init_gdt_entries();
	gdt_install(GDTMAX);

	_on_gdt_change(get_segment_selector_GDT(2,0), get_segment_selector_GDT(1,0));//data(ecx),ccode(edx)	
	_set_userspace_selectors(get_segment_selector_GDT(4,3), get_segment_selector_GDT(3,3));
	
	_tss_flush(get_segment_selector_GDT(5,3));
	//dputs(get_segment_selector_GDT(5,3));
}