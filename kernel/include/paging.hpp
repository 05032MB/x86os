#ifndef _PAGING_H
#define _PAGING_H
#include <types.hpp>
#include <vga.hpp>
#include <critical.hpp>
#include <interrupts.hpp>
#include <kmemory.hpp>
#include <mtracker.hpp>

#define PTSIZE 1024

#define ALIGNED_4kB __attribute__((aligned(4096)))
#define ALIGNED_4MB __attribute__((aligned(4194304)))

//extern ALIGNED_4kB dword page_directory[PTSIZE];
//extern ALIGNED_4kB dword *current_directory;

void init_paging();
void init_paging_phase_2();

//----------------------
	/*Page table ebtry structure explained
	-----------------------------------------------------------------------------------------------------------------------------------------
    | 31-12                                   |11...9		 |8		|7  |6		   |5	    |4	        |3			 |2			  		   |1		         |0	     |bits
    |Address of frame (lower 12 bits set to 0)|Bits for me   |Global|0  |DirtyBit  |Accessed|DisablCache|WriteThrough|Supervisor(0)/User(1)|Read(0)/Write(1) |Present|function
	-----------------------------------------------------------------------------------------------------------------------------------------
	*/
#define ENTR_GLOBAL(x) x<<8
#define ENTR_DIRTY(x) x<<6	


	/*Page directory entry structure explained
	-----------------------------------------------------------------------------------------------------------------------------------------------------------
    | 31-12                                   |11...9	   |8		  |7		 |6	|5		 |4	         |3			  |2		      |1		 |0	     |bits
    |Address          (lower 12 bits set to 0)|Bits for me |Ignored   |PageSize  |0	|Accessed|DisablCache|WriteThrough|User/Supervisor|Read/Write|Present|function
	-----------------------------------------------------------------------------------------------------------------------------------------------------------
	set 7 to 1 to align 4MiB, otherwise it's 4KiB aligned
	*/
#define ENTR_SIZE(x) x<<7
#define ENTR_ACCESSED(x) x<<5
#define ENTR_CACHE(x) x<<4
#define ENTR_WRITETHROUGH(x) x<<3
#define ENTR_USER(x) x<<2
#define ENTR_RW(x) x<<1
#define ENTR_PRESENT(x) x


#define ONLY_ADDR(x) (x & 0xFFFFF000)
#define ONLY_DATA(x) (x & 0xFFF)
//----------------Pewnie tego nie użyję
using page_t = dword;

struct page_table_t{
	page_t pages[PTSIZE];
};

struct page_dir_t{
	page_table_t *tables[PTSIZE]; //pointers to above
	addr_t physAddr[PTSIZE]; //physical address for tables
	addr_t selfPhys; //physical address for self
};

//---------------

//bool set_page_dir(dword);

void page_fault_handler(const int_iden ii);

void set_page_dir(void *pg);
void reset_global_pdir();

size_t fmemmap(addr_t virt, size_t size, word flags, mtracker *mt, dword * page_directory = nullptr);
//void destroy_aspace(dword * page_directory);

dword * clone_ptable(dword * pt);
dword * clone_pdir(dword * pd);


#endif