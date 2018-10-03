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
	for(int i = 0; i < PTSIZE; i++)page_table_primary[i] = (i * 0x1000) | 0xB; //supervisor, rw, present
	page_directory[0] |= ((dword)page_table_primary );
	page_directory[0] |= 0xB;
}

void * translate_virtual_to_physical(void * virtualaddr)
{
	//unsigned long pti = (unsigned long)virtualaddr >> 12;

	//void * addr = (void *)(pt[pti] & 
}

void init_frame()
{


}

void page_fault_handler(const int_iden ii)
{
	addr_t fault_loc;
	fault_loc = _get_cr2();
	
	
	
	kpanic("Page Fault: ");
	
}

void init_paging()
{

	populate_page_directory();
	init_primary_page_table();
	
	_write_cr3((dword)&page_directory);
	_write_cr0(0x80000000 | _get_cr0());
}