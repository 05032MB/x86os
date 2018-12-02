#include <paging.hpp>

ALIGNED_4kB//4KiB in bytes; 4KiB aligned
dword page_directory[PTSIZE]; //4KiB /32 = 1024

ALIGNED_4kB
dword page_table_primary[PTSIZE]; //maps first 4MB, for kernel, stack

#ifdef __k_debug
#include <stdio.h>
#include <stdlib.h>
__attribute__((used))
static void dputs(unsigned int a)
{
	char tr[50];
	itoa(a, tr, 16);
	puts(tr);
}
#endif

static void populate_page_directory()
{
	for(int i=0; i<PTSIZE; i++)
	{
		page_directory[i] = 2;//write enabled
	}
}

static void init_primary_page_table()
{
	page_directory[0] = 2 + 5;//write enabled
	
	for(int i = 0; i < PTSIZE; i++)page_table_primary[i] = (i * 0x1000) | 0xF;//0xB; //supervisor, rw, present
	page_directory[0] |= ((dword)page_table_primary );
	page_directory[0] |= 0xF;//0xB;
}
void * translate_virtual_to_physical(void * virtualaddr)
{
	//unsigned long pti = (unsigned long)virtualaddr >> 12;

	//void * addr = (void *)(pt[pti] & 
}

void* init_pagedir_entry(dword* dir_entry, word flags) //creates aligned ENTRY in page directory with specified FLAGS
{
	void *pointer = (void*)sysheap.__aalloc(sizeof(dword)*PTSIZE,0x1000);
	
	*dir_entry = (dword)((to_addr_t(pointer)  & ~(0xFFF)) | flags);
	
	return pointer;

}

void init_frame(/*addr_t virtual_addr*/dword* tab_entry, addr_t phys_addr, word flags)
{
	//dword *pointer = (dword*)sysheap.__aalloc(sizeof(frame),0x1000);
	
	//virtual_addr = virtual_addr & ~(0xFFF); //assure it's 4KiB aligned
	phys_addr = phys_addr & ~(0xFFF); //assure it's 4KiB aligned
	
	*tab_entry = (dword)((to_addr_t(phys_addr)  & ~(0xFFF)) | flags);
	

}


void page_fault_handler(const int_iden ii)
{
	addr_t fault_loc;
	fault_loc = _get_cr2();
	
	
	
	#ifdef __k_debug
	term_print("Page Fault debug information:");
	term_print_hex(fault_loc);
	kpanic("Page Fault");
	#endif //i assume that there would be no need to panic after more coding
}

void init_paging()
{
	//populate_page_directory();
	init_primary_page_table();
	
	_write_cr3((dword)&page_directory);
	_write_cr0(0x80000000 | _get_cr0());
}

__attribute__((optimize("-O0")))
static void poke_addr(addr_t addr)
{
	int* pointer = (int*)addr;
	//dputs((unsigned int)pointer);
	int bckp = *pointer;
	*pointer = 5;
	*pointer = bckp;

}

void init_paging_phase_2()
{	

	/*dword* page_table_2 = (dword*)init_pagedir_entry(&page_directory[1], 0xB);
	init_frame(&page_table_2[0], 0x400000 , 0xB); */
	dword* page_table_2 = (dword*)init_pagedir_entry(&page_directory[1], 0xF);//krnl
	for(int i =0; i<1024; i++)init_frame(&page_table_2[i], 0x400000 + i*4*1024 , 0xF); //[4MB;8MB), system memory
	
	dword* page_table_3 = (dword*)init_pagedir_entry(&page_directory[2], 0xF);//user
	for(int i =0; i<1024; i++)init_frame(&page_table_3[i], 0xA00000 + i*4*1024 , 0xF); //[10MB;14MB), user memory
	
	/*poke_addr(0xA00001);//ok
	poke_addr(0x400000);//ok
	poke_addr(0x799999);
	poke_addr(0x800001);//?
	poke_addr(0xA00000-0x2);//?
	//poke_addr(0xFFFFFFF);*/
	

}